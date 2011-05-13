/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include "Type.hpp"
#include "Feature.hpp"
#include "Environment.hpp"
#include <cassert>

Type::Type(Location loc, Name* sc, Generic* gen, Module* mod, Environment* env) :
    TreeNode(loc),
    scope_(sc),
    generics_(gen),
    module_(mod),
    environment_(env) {

    size_t scope_end = scope_->string().find_last_of(':');
    if (scope_end == std::string::npos) {
        name_ = scope_;
    } else {
        name_ = env->name(scope_->string().substr(scope_end + 1));
    }

}

bool Type::equals(Type* other) const {
    /* Make sure the classes are equal */
    if (clazz() != other->clazz()) {
        return false;
    }

    /* Make sure the generic parameters are the same */
    Generic* g1 = generics();
    Generic* g2 = other->generics();
    while (g1 && g2) {
        if (g1->type()->equals(g2->type())) {
            return false;
        }
        
        g1 = g1->next();
        g2 = g2->next();
    }
    return true;
}

bool Type::subtype(Type* other) const {
    if (!clazz() || !other->clazz()) {
        return false;
    }
    return clazz()->subtype(other->clazz());
}

bool Type::supertype(Type* other) const {
    if (!clazz() || !other->clazz()) {
        return false;
    }
    return clazz()->supertype(other->clazz());
}

Class* Type::clazz() const {
    if (clazz_) {
        return clazz_;
    }
    Type* self = const_cast<Type*>(this);
    self->clazz_ = self->module_->clazz(name_);

    return 0;
}


std::ostream& operator<<(std::ostream& out, const Type* type) {
    out << type->scope()->string();
    if (type->generics()) {
        out << '[';
        for (Generic::Ptr g = type->generics(); g; g = g->next()) {
            out << g->type();
            if (g->next()) {
                out << ',';
            }
        }
        out << ']';
    } 
    return out;
}
