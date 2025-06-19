/**
 * Frequency-based replacement policy (LFU + FIFO tie-break).
 * Victim = entry with the smallest refCount; if equal, choose the
 * one inserted furthest in the past (tickInserted is min).
 */

#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_FREQ_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_FREQ_RP_HH__

#include "base/types.hh"
#include "mem/cache/replacement_policies/base.hh"

struct FreqRPParams;

class FreqRP : public BaseReplacementPolicy
{
  protected:
    /** Per-block replacement metadata. */
    struct FreqReplData : ReplacementData
    {
        unsigned refCount;   //!< # of touches since (re)insert
        Tick     tickInserted; //!< insertion time stamp

        FreqReplData() : refCount(0), tickInserted(0) {}
    };

  public:
    typedef FreqRPParams Params;
    FreqRP(const Params *p);
    ~FreqRP() {}

    /** BaseReplacementPolicy API **/
    void invalidate(const std::shared_ptr<ReplacementData>& rd) const override;
    void touch     (const std::shared_ptr<ReplacementData>& rd) const override;
    void reset     (const std::shared_ptr<ReplacementData>& rd) const override;
    ReplaceableEntry* getVictim(const ReplacementCandidates& cands) const override;
    std::shared_ptr<ReplacementData> instantiateEntry() override;
};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_FREQ_RP_HH__

