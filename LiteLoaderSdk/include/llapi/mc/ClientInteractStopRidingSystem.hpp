/**
 * @file  ClientInteractStopRidingSystem.hpp
 *
 */
#pragma once
#define AUTO_GENERATED
#include "llapi/Global.h"

#define BEFORE_EXTRA

#undef BEFORE_EXTRA

/**
 * @brief MC class ClientInteractStopRidingSystem.
 *
 */
class ClientInteractStopRidingSystem {

#define AFTER_EXTRA

#undef AFTER_EXTRA
#ifndef DISABLE_CONSTRUCTOR_PREVENTION_CLIENTINTERACTSTOPRIDINGSYSTEM
public:
    class ClientInteractStopRidingSystem& operator=(class ClientInteractStopRidingSystem const &) = delete;
    ClientInteractStopRidingSystem(class ClientInteractStopRidingSystem const &) = delete;
    ClientInteractStopRidingSystem() = delete;
#endif

public:
    /**
     * @symbol ?createSystem\@ClientInteractStopRidingSystem\@\@SA?AUTickingSystemWithInfo\@\@XZ
     */
    MCAPI static struct TickingSystemWithInfo createSystem();

};
