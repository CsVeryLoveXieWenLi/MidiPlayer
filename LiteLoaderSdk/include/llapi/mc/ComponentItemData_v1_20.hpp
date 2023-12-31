/**
 * @file  ComponentItemData_v1_20.hpp
 *
 */
#pragma once
#define AUTO_GENERATED
#include "llapi/Global.h"

#define BEFORE_EXTRA

#undef BEFORE_EXTRA


struct ComponentItemData_v1_20 {

#define AFTER_EXTRA

#undef AFTER_EXTRA
#ifndef DISABLE_CONSTRUCTOR_PREVENTION_COMPONENTITEMDATA_V1_20
public:
    struct ComponentItemData_v1_20& operator=(struct ComponentItemData_v1_20 const &) = delete;
#endif

public:
    /**
     * @symbol ??0ComponentItemData_v1_20\@\@QEAA\@AEBU0\@\@Z
     */
    MCAPI ComponentItemData_v1_20(struct ComponentItemData_v1_20 const &);
    /**
     * @symbol ??0ComponentItemData_v1_20\@\@QEAA\@XZ
     */
    MCAPI ComponentItemData_v1_20();
    /**
     * @symbol ??0ComponentItemData_v1_20\@\@QEAA\@$$QEAU0\@\@Z
     */
    MCAPI ComponentItemData_v1_20(struct ComponentItemData_v1_20 &&);
    /**
     * @symbol ??1ComponentItemData_v1_20\@\@QEAA\@XZ
     */
    MCAPI ~ComponentItemData_v1_20();
    /**
     * @symbol ?VERSION\@ComponentItemData_v1_20\@\@2VSemVersion\@\@B
     */
    MCAPI static class SemVersion const VERSION;
    /**
     * @symbol ?bindType\@ComponentItemData_v1_20\@\@SAXXZ
     */
    MCAPI static void bindType();
    /**
     * @symbol ?upgrade\@ComponentItemData_v1_20\@\@SA?AU1\@AEAUComponentItemData_v1_19_83\@\@\@Z
     */
    MCAPI static struct ComponentItemData_v1_20 upgrade(struct ComponentItemData_v1_19_83 &);

};