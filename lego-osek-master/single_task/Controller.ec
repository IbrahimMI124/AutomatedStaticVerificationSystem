const
  rotate_speed : real;
  rotate_speed_neg : real;
  white_treshold : real;
  black_treshold : real;
  obstacle_treshold : real;
  pi : real;
  kp_teta : real;
  ki_teta : real;
  
node Controller
  (Cg: real;
  Cd: real;
  Co: real)
returns
  (u_d: real;
  u_g: real);

var
  V17_w_002: bool;
  V19_w_004: real;
  V21_w_006: real;
  V23_w_008: bool;
  V24_w_009: bool;
  V26_w_011: real;
  V28_w_013: real;
  V309_w_001: bool;
  V310_w_002: bool;
  V311_w_003: bool;
  V312_w_004: bool;
  V313_w_000: bool;
  V314_w_000: bool;
  V315_w_004: bool;
  V316_w_005: bool;
  V317_w_000: bool;
  V318_w_000: bool;
  V319_w_004: bool;
  V320_w_005: bool;
  V321_w_000: bool;
  V322_w_000: bool;
  V323_w_004: bool;
  V324_w_005: bool;
  V333_w_000: bool;
  V334_w_001: bool;
  V335_w_004: bool;
  V336_w_005: bool;
  V349_w_004: real;
  V371_w_003: real;
  V372_w_004: real;
  V373_w_005: real;
  V374_w_006: real;
  V375_w_007: real;
  V376_w_008: real;
  V377_w_009: real;
  V378_w_011: real;
  V379_w_012: real;
  V380_w_013: real;
  V381_w_014: real;
  V382_w_015: real;
  V383_w_016: real;

let
  u_d = (if V17_w_002 then rotate_speed else V21_w_006);
  u_g = (if V17_w_002 then rotate_speed_neg else V19_w_004);
  V17_w_002 = (V336_w_005 and V334_w_001);
  V19_w_004 = ((1.000000 / 2.000000) * V380_w_013);
  V21_w_006 = ((1.000000 / 2.000000) * V381_w_014);
  V23_w_008 = (false -> (pre V24_w_009));
  V24_w_009 = (V309_w_001 and V310_w_002);
  V26_w_011 = ((pi / 200.000000) * V349_w_004);
  V28_w_013 = (Cd + Cg);
  V309_w_001 = (V320_w_005 and V318_w_000);
  V310_w_002 = (V312_w_004 and V311_w_003);
  V311_w_003 = (V316_w_005 and V314_w_000);
  V312_w_004 = (V324_w_005 and V322_w_000);
  V313_w_000 = (Cg <= white_treshold);
  V314_w_000 = (not V23_w_008);
  V315_w_004 = (false -> (pre V311_w_003));
  V316_w_005 = (V313_w_000 or V315_w_004);
  V317_w_000 = (Cg >= white_treshold);
  V318_w_000 = (not V23_w_008);
  V319_w_004 = (false -> (pre V309_w_001));
  V320_w_005 = (V317_w_000 or V319_w_004);
  V321_w_000 = (Cg <= black_treshold);
  V322_w_000 = (not V23_w_008);
  V323_w_004 = (false -> (pre V312_w_004));
  V324_w_005 = (V321_w_000 or V323_w_004);
  V333_w_000 = (Co < obstacle_treshold);
  V334_w_001 = (not V23_w_008);
  V335_w_004 = (false -> (pre V17_w_002));
  V336_w_005 = (V335_w_004 or V333_w_000);
  V349_w_004 = (Cd - Cg);
  V371_w_003 = (0.000000 -> (pre V372_w_004));
  V372_w_004 = (V373_w_005 + V371_w_003);
  V373_w_005 = (0.002000 * V26_w_011);
  V374_w_006 = (V383_w_016 + V382_w_015);
  V375_w_007 = ((-V377_w_009) + V378_w_011);
  V376_w_008 = (2.000000 * V375_w_007);
  V377_w_009 = (if (V379_w_012 < 0.000000) then (-V379_w_012) else V379_w_012);
  V378_w_011 = (0.005000 * V28_w_013);
  V379_w_012 = (2.000000 * V374_w_006);
  V380_w_013 = ((-V374_w_006) + V376_w_008);
  V381_w_014 = (V374_w_006 + V376_w_008);
  V382_w_015 = (kp_teta * V26_w_011);
  V383_w_016 = (ki_teta * V372_w_004);
tel

