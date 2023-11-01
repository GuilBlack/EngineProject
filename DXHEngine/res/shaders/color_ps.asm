//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float       
// COLOR                    0   xyzw        1     NONE   float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Target                0   xyzw        0   TARGET   float   xyzw
//
ps_5_0
dcl_globalFlags refactoringAllowed | skipOptimization
dcl_input_ps linear v1.xyzw
dcl_output o0.xyzw
//
// Initial variable locations:
//   v0.x <- pIn.PosH.x; v0.y <- pIn.PosH.y; v0.z <- pIn.PosH.z; v0.w <- pIn.PosH.w; 
//   v1.x <- pIn.Col.x; v1.y <- pIn.Col.y; v1.z <- pIn.Col.z; v1.w <- pIn.Col.w; 
//   o0.x <- <PS return value>.x; o0.y <- <PS return value>.y; o0.z <- <PS return value>.z; o0.w <- <PS return value>.w
//
#line 44 "C:\dev\DirectX\DirectXBasicRendering\DirectXBasicRendering\res\Shader\color.hlsl"
mov o0.xyzw, v1.xyzw
ret 
// Approximately 2 instruction slots used