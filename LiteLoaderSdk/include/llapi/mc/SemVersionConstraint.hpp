/**
 * @file  SemVersionConstraint.hpp
 *
 */
#pragma once
#define AUTO_GENERATED
#include "llapi/Global.h"

#define BEFORE_EXTRA

#undef BEFORE_EXTRA

/**
 * @brief MC class SemVersionConstraint.
 *
 */
class SemVersionConstraint {

#define AFTER_EXTRA

#undef AFTER_EXTRA
#ifndef DISABLE_CONSTRUCTOR_PREVENTION_SEMVERSIONCONSTRAINT
public:
    class SemVersionConstraint& operator=(class SemVersionConstraint const &) = delete;
    SemVersionConstraint(class SemVersionConstraint const &) = delete;
    SemVersionConstraint() = delete;
#endif

public:
    /**
     * @vftbl 0
     * @symbol ?doValidate\@SemVersionConstraint\@\@EEBA_NAEBVmeta_any\@entt\@\@AEAVSerializerContext\@cereal\@\@\@Z
     */
    virtual bool doValidate(class entt::meta_any const &, class cereal::SerializerContext &) const;
#ifdef ENABLE_VIRTUAL_FAKESYMBOL_SEMVERSIONCONSTRAINT
    /**
     * @symbol __unk_destructor_-1
     */
    MCVAPI ~SemVersionConstraint();
#endif
    /**
     * @symbol ?min\@SemVersionConstraint\@\@QEAAAEAV1\@AEBVSemVersion\@\@\@Z
     */
    MCAPI class SemVersionConstraint & min(class SemVersion const &);

};
