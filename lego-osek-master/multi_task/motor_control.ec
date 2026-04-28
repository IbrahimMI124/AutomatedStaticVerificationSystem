const
  pi : real;
  kp_teta : real;
  ki_teta : real;
  
node motor_control
  (Cg: real;
  Cd: real)
returns
  (u_d: real;
  u_g: real);

var
  V12_w_003: real;
  V14_w_005: real;
  V85_w_004: real;
  V104_w_003: real;
  V105_w_004: real;
  V106_w_005: real;
  V107_w_006: real;
  V108_w_007: real;
  V109_w_008: real;
  V110_w_009: real;
  V111_w_011: real;
  V112_w_012: real;
  V113_w_013: real;
  V114_w_014: real;
  V115_w_015: real;
  V116_w_016: real;

let
  u_d = ((1.000000 / 2.000000) * V114_w_014);
  u_g = ((1.000000 / 2.000000) * V113_w_013);
  V12_w_003 = ((pi / 200.000000) * V85_w_004);
  V14_w_005 = (Cd + Cg);
  V85_w_004 = (Cd - Cg);
  V104_w_003 = (0.000000 -> (pre V105_w_004));
  V105_w_004 = (V106_w_005 + V104_w_003);
  V106_w_005 = (0.002000 * V12_w_003);
  V107_w_006 = (V116_w_016 + V115_w_015);
  V108_w_007 = ((-V110_w_009) + V111_w_011);
  V109_w_008 = (2.000000 * V108_w_007);
  V110_w_009 = (if (V112_w_012 < 0.000000) then (-V112_w_012) else V112_w_012);
  V111_w_011 = (0.005000 * V14_w_005);
  V112_w_012 = (2.000000 * V107_w_006);
  V113_w_013 = ((-V107_w_006) + V109_w_008);
  V114_w_014 = (V107_w_006 + V109_w_008);
  V115_w_015 = (kp_teta * V12_w_003);
  V116_w_016 = (ki_teta * V105_w_004);
tel

