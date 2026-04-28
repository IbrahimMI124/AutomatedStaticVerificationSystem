const
  white_threshold : real;
  black_threshold : real;
  obstacle_threshold : real;
  
node obst_detector
  (Co: real;
  Cg: real)
returns
  (obstacle: bool);

var
  V11_w_003: bool;
  V12_w_004: bool;
  V238_w_001: bool;
  V239_w_002: bool;
  V240_w_003: bool;
  V241_w_004: bool;
  V242_w_000: bool;
  V243_w_000: bool;
  V244_w_004: bool;
  V245_w_005: bool;
  V246_w_000: bool;
  V247_w_000: bool;
  V248_w_004: bool;
  V249_w_005: bool;
  V250_w_000: bool;
  V251_w_000: bool;
  V252_w_004: bool;
  V253_w_005: bool;
  V262_w_000: bool;
  V263_w_000: bool;
  V264_w_004: bool;
  V265_w_005: bool;

let
  obstacle = (V265_w_005 and V263_w_000);
  V11_w_003 = (false -> (pre V12_w_004));
  V12_w_004 = (V238_w_001 and V239_w_002);
  V238_w_001 = (V249_w_005 and V247_w_000);
  V239_w_002 = (V241_w_004 and V240_w_003);
  V240_w_003 = (V245_w_005 and V243_w_000);
  V241_w_004 = (V253_w_005 and V251_w_000);
  V242_w_000 = (Cg <= white_threshold);
  V243_w_000 = (not V11_w_003);
  V244_w_004 = (false -> (pre V240_w_003));
  V245_w_005 = (V242_w_000 or V244_w_004);
  V246_w_000 = (Cg >= white_threshold);
  V247_w_000 = (not V11_w_003);
  V248_w_004 = (false -> (pre V238_w_001));
  V249_w_005 = (V246_w_000 or V248_w_004);
  V250_w_000 = (Cg <= black_threshold);
  V251_w_000 = (not V11_w_003);
  V252_w_004 = (false -> (pre V241_w_004));
  V253_w_005 = (V250_w_000 or V252_w_004);
  V262_w_000 = (Co < obstacle_threshold);
  V263_w_000 = (not V11_w_003);
  V264_w_004 = (false -> (pre obstacle));
  V265_w_005 = (V262_w_000 or V264_w_004);
tel

