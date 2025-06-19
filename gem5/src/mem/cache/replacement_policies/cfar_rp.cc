/**
 * Copyright (c) 2018 Inria
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Daniel Carvalho
 */

#include "mem/cache/replacement_policies/cfar_rp.hh"

#include <cassert>
#include <memory>

#include "params/CFARRP.hh"

CFARRP::CFARRP(const Params *p)
    : LRURP(p)
{
}

void
CFARRP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
const
{
    // Reset last touch timestamp
    auto data = std::static_pointer_cast<CFARReplData>(replacement_data);
    data->lastTouchTick = Tick(0);
    data->priority = false;
}

void
CFARRP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Update last touch timestamp
    std::static_pointer_cast<CFARReplData>(
        replacement_data)->lastTouchTick = curTick();
}

void
CFARRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Set last touch timestamp
    auto data = std::static_pointer_cast<CFARReplData>(replacement_data);
    data->lastTouchTick = curTick();
    data->priority = false;
}

ReplaceableEntry*
CFARRP::getVictim(const ReplacementCandidates& candidates) const
{
    // There must be at least one replacement candidate
    assert(candidates.size() > 0);

    ReplaceableEntry* victim = nullptr;
    // First try to find a clean candidate
    for (const auto& candidate : candidates) {
        auto data = std::static_pointer_cast<CFARReplData>(candidate->replacementData);
        if (!data->priority) {
            if (!victim || data->lastTouchTick <
                    std::static_pointer_cast<CFARReplData>(victim->replacementData)->lastTouchTick) {
                victim = candidate;
            }
        }
    }
    if (!victim) {
        victim = LRURP::getVictim(candidates);
    }
    return victim;
}

std::shared_ptr<ReplacementData>
CFARRP::instantiateEntry()
{
    return std::make_shared<CFARReplData>();
}
void
CFARRP::markDirty(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    std::static_pointer_cast<CFARReplData>(replacement_data)->priority = true;
}

void
CFARRP::clearDirty(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    std::static_pointer_cast<CFARReplData>(replacement_data)->priority = false;
}

CFARRP*
CFARRPParams::create()
{
    return new CFARRP(this);
}
