/*
** EPITECH PROJECT, 2019
** GameAI.hpp
** File description:
** 
*/

#pragma once

// oA::Thread
#include "Core/Thread.hpp"

#include "GamePlayer.hpp"

class GameCore;

enum BehaviourEnum {
    Undefined,
    Offensive,
    Defensive,
    Dynamic
};

enum BehaviourState {
    Thinking,
    Working
};

class GameAI {
    public:
        GameAI(GameCore &core, oA::Uint indexPlayer, BehaviourEnum = Undefined);
        ~GameAI(void);

        /* Move semantic */
        GameAI(GameAI &&other);

        void run(void);
        void stop(void);

    private:
        oA::Thread thd;
        GameCore &core;
        oA::Uint player_index;
        oA::V2u goal;
        oA::V2u current;
        oA::V2u old;
        BehaviourState state;
        BehaviourEnum behaviour;
        bool running;
        bool fruct;
        bool bomb;
        int len;

        /**
         * @brief Wait for given milliseconds while watching at running state
         *
         * @param ms Milliseconds to wait
         * @return true AI is still running after wait
         * @return false AI has been disconnected from remote
         */
        bool waitFor(oA::Uint ms);

        void IAMove(void);
        void IAWork(void);

        PlayerAction GetDirectionBetweenPoints(const oA::V2u &p1, const oA::V2u &p2);
        oA::V2u GetNearestNoDangerousCell(void);
        bool CanMove(oA::V2u);
        void TryBomb(void);
        void SetNewGoal(void);
        void SetNewGoalOffense(void);
        void SetNewGoalDefense(void);
        bool ValidCell(const oA::V2u &pos);
        bool IsDangerous(const oA::V2u &pos);
        bool IsSolidBlock(const oA::V2u &pos);
};
