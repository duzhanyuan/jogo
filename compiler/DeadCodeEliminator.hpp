/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
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

#include "Jogo.hpp"
#include "Environment.hpp"
#include "IrBlock.hpp"
#include "LivenessAnalyzer.hpp"
#include "Object.hpp"

/* Eliminates dead code from functions */
class DeadCodeEliminator : public TreeNode::Functor {
public:
    DeadCodeEliminator(Environment* env, Machine* mach);
    
    typedef Pointer<DeadCodeEliminator> Ptr; 
    void operator()(File* file);
    
private:
    void operator()(Class* feature);
    void operator()(Function* feature);
    void operator()(IrBlock* block);

    Environment::Ptr env_;
    LivenessAnalyzer::Ptr liveness_;
    int eliminated_;
};
