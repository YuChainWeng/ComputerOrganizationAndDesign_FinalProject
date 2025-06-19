#include "mem/cache/replacement_policies/freq_rp.hh"

#include <cassert>
#include <memory>

#include "params/FreqRP.hh"

FreqRP::FreqRP(const Params *p)
    : BaseReplacementPolicy(p)
{ }

void
FreqRP::invalidate(const std::shared_ptr<ReplacementData>& rd) const
{
    auto data = std::static_pointer_cast<FreqReplData>(rd);
    data->refCount     = 0;
    data->tickInserted = Tick(0);
}

void
FreqRP::touch(const std::shared_ptr<ReplacementData>& rd) const
{
    std::static_pointer_cast<FreqReplData>(rd)->refCount++;
}

void
FreqRP::reset(const std::shared_ptr<ReplacementData>& rd) const
{
    auto data = std::static_pointer_cast<FreqReplData>(rd);
    data->refCount     = 1;        // first reference on insertion
    data->tickInserted = curTick();
}

ReplaceableEntry*
FreqRP::getVictim(const ReplacementCandidates& cands) const
{
    assert(!cands.empty());

    ReplaceableEntry* victim = cands[0];
    for (auto* cand : cands) {
        auto cData = std::static_pointer_cast<FreqReplData>(cand->replacementData);
        auto vData = std::static_pointer_cast<FreqReplData>(victim->replacementData);

        if (cData->refCount < vData->refCount ||
           (cData->refCount == vData->refCount &&
            cData->tickInserted < vData->tickInserted)) {
            victim = cand;
        }
    }
    return victim;
}

std::shared_ptr<ReplacementData>
FreqRP::instantiateEntry()
{
    return std::make_shared<FreqReplData>();
}

/* Factory */
FreqRP*
FreqRPParams::create()
{
    return new FreqRP(this);
}

