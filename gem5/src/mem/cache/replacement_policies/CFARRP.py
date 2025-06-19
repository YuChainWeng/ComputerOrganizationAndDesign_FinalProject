from m5.params import *
from ReplacementPolicies import LRURP

class CFARRP(LRURP):
    type = 'CFARRP'
    cxx_class = 'CFARRP'
    cxx_header = 'mem/cache/replacement_policies/cfar_rp.hh'
