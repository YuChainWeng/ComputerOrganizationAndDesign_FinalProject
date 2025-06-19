from m5.params import *
from ReplacementPolicies import BaseReplacementPolicy

class CFARRP(BaseReplacementPolicy):
    type = 'CFARRP'
    cxx_header = 'mem/cache/replacement_policies/cfar_rp.hh'
