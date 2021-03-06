/* QuizBoard - QuizGame.h
 * ========================================================================
 * Copyright 2013 Joerg Desch <github@jdesch.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * ========================================================================
 *
 * This class implements logic of the quiz game. It handles the different
 * answer sets, checks the answers and keep track of all answered questions.
 *
 * Note: the API of his class counts all answers and questions from 1.
 *
 * ========================================================================
 * TODO:
 *
 * ========================================================================
 */
#ifndef __QB_QUIZGAME_HXX__
#define __QB_QUIZGAME_HXX__

/* These local include files must be opened in a tab. If not, the headers
 * could not be found! Don't aks me why...
 */
#include "config.h"

#include <stdarg.h>
#include <Arduino.h>

#include "SimpleLogging.h"

#ifndef CFG_MAX_ANSWERS
#  error "fatal: CFG_MAX_ANSWERS must be defined in config.h"
#endif
#ifndef CFG_MAX_QUESTIONS
#  error "fatal: CFG_MAX_QUESTIONS must be defined in config.h"
#endif
#ifndef CFG_MAX_SOLUTIONSETS
#  error "fatal: CFG_MAX_SOLUTIONSETS must be defined in config.h"
#endif


/* The "Quiz Game " class
 *
 */
class QuizGame
{
  /* --------------------------------------------
   * inner data types
   */
  private:
    struct SolutionSet { int CorrectAnswer[CFG_MAX_QUESTIONS]; };

  public:
    enum RESULTS {RES_ERROR=(-1), RES_NONE=0, RES_WRONG, RES_CORRECT };

  /* --------------------------------------------
   * constants declarations
   */
  //private:
  //public:

  /* --------------------------------------------
   * local attributes
   */
  protected:
    static SolutionSet m_SolutionSet[CFG_MAX_SOLUTIONSETS];
    int m_GivenAnswer[CFG_MAX_QUESTIONS];	// the given answers. 0=unanswered
    int m_NumUnAnswered;			// number of questions without an answer
    int m_UseSet;				// used answer set
    bool m_GameRunning;
    SimpleLogging m_dbg;

/* --------------------------------------------
   * methods
   */
  protected:


  public:

    QuizGame();
    void stopGame ( void );
    void startGame ( int AnswerSet );
    bool gameFinished ( void );
    bool giveAnswer ( int question, int answer );
    int questionAnswered ( int question );
    bool success ( void );
    bool gameRunning (void) {return m_GameRunning;};
};


/* If this define is defined, we can allocate our code and static data here.
 * This way we avoid libraries and additional CXX files.
 */
#ifdef __ALLOC_STATICS_HERE__

QuizGame::QuizGame()
{
#ifdef DEBUG
    m_dbg.setLevel(SimpleLogging::LVL_DEBUG);
#else
    m_dbg.setLevel(SimpleLogging::LVL_WARNING);
#endif
    stopGame();
}

void QuizGame::stopGame ( void )
{
    m_GameRunning = false;
    m_UseSet = 0;
    m_NumUnAnswered = CFG_MAX_QUESTIONS;
    for ( int i=0; i<CFG_MAX_QUESTIONS; i++ )
	m_GivenAnswer[i] = 0;
}

void QuizGame::startGame ( int AnswerSet )
{
    m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGame::startGame");
    if ( AnswerSet<0 || AnswerSet>=CFG_MAX_SOLUTIONSETS )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"QuizGame::startGame: invalid AnswerSet! use default.");
	m_UseSet = 0;
    }
    else
        m_UseSet = AnswerSet;
    m_NumUnAnswered = CFG_MAX_QUESTIONS;
    for ( int i=0; i<CFG_MAX_QUESTIONS; i++ )
	m_GivenAnswer[i] = 0;
    m_GameRunning = true;
}

/* Returns true is the game is finished. The game is finished, if there
 * are no more unanswered questions left.
 */
bool QuizGame::gameFinished ( void )
{
    return (m_NumUnAnswered == 0) ? true:false;
}

/* Pass a given answer to the game. The method returns false, if the answer
 * is not excepted. After counting a valid answer, the answer is stored
 * Use questionAnswered() to get the result. Use gameFinished() to check if the
 * the game is done.
 */
bool QuizGame::giveAnswer ( int question, int answer )
{
    if ( !m_GameRunning )
    {
	m_dbg.log(SimpleLogging::LVL_WARNING,"QuizGame::giveAnswer: Game not running.");
	return false;
    }
    if ( question<=0 || question>CFG_MAX_QUESTIONS )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"QuizGame::giveAnswer: invalid question.");
	return false;
    }
    if ( answer<=0 || answer>CFG_MAX_ANSWERS )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"QuizGame::giveAnswer: invalid answer.");
	return false;
    }
    if ( m_NumUnAnswered==0 )
    {
	m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGame::giveAnswer: nothing to do");
	return true;
    }

    // we need an index which count from zero.
    --question;

    if ( m_GivenAnswer[question]==0 )		// store only if not yet answered
    {
	m_GivenAnswer[question] = answer;	// the the answer
	--m_NumUnAnswered;			// and count it.
    }
    else
    {
	m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGame::giveAnswer: question already answered");
    }
    return true;
}

/* Check if the given question is answered and if the answer is correct.
 * The returned value is a RES_* code.
 *
 *   RES_ERROR    =>  (-1) error. invalid parameter
 *   RES_NONE     =>  (0)  question isn't answered yet
 *   RES_WRONG    =>  (1)  wrong answer
 *   RES_CORRECT  =>  (2)  correct answer
 */
int QuizGame::questionAnswered ( int question )
{
    int CorrectAnswer;

    if ( question<=0 || question>CFG_MAX_QUESTIONS )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"QuizGame::questionAnswered: invalid parameter.");
	return RES_ERROR;
    }
    // we need an index which count from zero.
    --question;

    // check if answered yet
    if ( m_GivenAnswer[question]==0 )
	return RES_NONE;

    // compare the result
    CorrectAnswer = m_SolutionSet[m_UseSet].CorrectAnswer[question];
    // m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGame::questionAnswered: %d==%d?",m_GivenAnswer[question],CorrectAnswer);
    return (m_GivenAnswer[question]==CorrectAnswer) ? RES_CORRECT:RES_WRONG;
}

/* The game is won, if all questions are answered. Simple, isn't it. ;-)
 */
bool QuizGame::success ( void )
{
    for ( int i=1; i<=CFG_MAX_QUESTIONS; i++ )
    {
	if ( questionAnswered(i)!=RES_CORRECT )
	    return false;
    }
    return true;
}


QuizGame::SolutionSet QuizGame::m_SolutionSet[CFG_MAX_SOLUTIONSETS] =
{
#ifdef DEBUG
    // these sets are just debugging sets...
    {1,2,3,4,1,2,3,4},
    {4,3,2,1,4,3,2,1},
    {1,2,1,2,3,4,3,4},
    {4,3,4,3,2,1,2,1}
#else
    // set #0
    {4,1,4,3,2,1,4,1},
    // set #1
    {4,3,3,1,1,3,1,2},
    // set #2
    {1,3,2,2,1,2,4,3},
    // set #3
    {1,3,1,1,4,4,1,3}
#endif
};

#endif // __ALLOC_STATICS_HERE__
#endif // __QB_QUIZGAME_HXX__
