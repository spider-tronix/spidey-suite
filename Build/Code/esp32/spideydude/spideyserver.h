#ifndef __SPIDYDUDE_SERVER_H__
#define __SPIDYDUDE_SERVER_H__

String start = "<!doctype html> <html> <head> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title>Spideydude</title> ";
String style = "<style> * { box-sizing: border-box; position: relative; }html,body{ width:100%; position: relative; margin: 0; padding: 0; text-align:center; font-family: 'Poppins'; overflow: auto; background: #333; min-width: 380px !important; }.container{ font-family: 'Poppins'; position: relative; width:100%; min-height: 100vh; top:0;left:0; background: #333; text-align: center; z-index: 2; }.loginWrapper{ min-width:350px; padding:20px 50px; background: #fff; border-radius: 10px; display: inline-flex; position: absolute; top:50%;left:50%; transform:translate(-50%,-50%); } .loginWrapper h1{ margin-bottom: 10px; user-select: none; color:#777; }.pinContainer{ display: block; width:100%; margin-top:20px; }#file-input { display: block; border: 1px solid #bbb; padding: 10px; border-radius: 5px; text-align: left; color: #aaa; width: 100%; cursor: pointer; transition: all 0.5s; } #file-input:hover { border-color: #777; color: #777; } .btn{ background:#3498db; color:#fff; padding:15px 10px; width:100%; border-radius: 5px; margin-top:15px; font-size:18px; font-weight:bold; transition:all 0.5s; border:none; outline: none; cursor: pointer; } .btn:hover{ background:rgba(52, 152, 219, 0.8); } .btn:disabled{ cursor: not-allowed; } #prg{ font-size:15px; color:#777; user-select: none; margin-bottom:7px; } #bar,#prgbar{ background-color:#f1f1f1; border-radius:10px } #bar{ background-color:#3498db; width:0%; height:10px } .svg_container svg{ width:100px; height:90px; }.window { display: none; width:80%; min-width:380px; padding:0px 30px; background: #fff; border-radius: 10px; position: absolute; top:50%;left:50%; transform:translate(-50%,-50%); z-index: 5; box-shadow: 0px 0px 20px 0px rgba(0,0,0,0.5); }.consoleWindow{ background: #000; width:100%; color: #fff; display: block; text-align: left; padding: 10px; margin-bottom: 1.5em; font-size: 15px; font-family: consolas; overflow-x: auto; } .con_info{ margin: 10px; } .ckere { cursor: pointer; font-weight: bold; transition: all 0.5s; } .ckere:hover { text-decoration: underline; } @media(max-width:650px){ .loginWrapper{padding:20px;} .window{ padding:0px 18px;} } </style> </head>";
String body = " <body> <div class=\"container\"> <div class=\"loginWrapper\"> <div style=\"position: relative;margin: auto;width: 100%;\"> <div class=\"svg_container\"> <svg version=\"1.0\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 1213.000000 1296.000000\" preserveAspectRatio=\"xMidYMid meet\"> <g transform=\"translate(0.000000,1296.000000) scale(0.100000,-0.100000)\" fill=\"#777\" stroke=\"none\"> <path d=\"M7220 12924 c-52 -20 -106 -39 -120 -44 -69 -24 -147 -52 -165 -60 -11 -4 -85 -32 -165 -60 -80 -29 -188 -68 -240 -87 -52 -19 -133 -48 -180 -65 -47 -16 -107 -38 -135 -49 -27 -10 -113 -40 -190 -68 -77 -27 -171 -61 -210 -75 -38 -15 -108 -40 -155 -56 -206 -73 -344 -127 -347 -137 -2 -6 10 -16 27 -23 17 -7 33 -16 36 -21 4 -8 -60 -138 -131 -264 -13 -22 -50 -92 -83 -155 -33 -63 -93 -178 -134 -255 -41 -77 -112 -212 -158 -300 -46 -88 -100 -189 -120 -225 -20 -36 -56 -103 -80 -150 -24 -47 -53 -103 -65 -125 -12 -22 -45 -85 -75 -140 -29 -55 -73 -138 -98 -185 l-45 -85 11 -95 c6 -52 21 -158 32 -235 11 -77 27 -187 35 -245 8 -58 23 -166 35 -240 11 -74 25 -171 30 -215 6 -44 15 -105 20 -135 6 -30 17 -107 26 -170 50 -369 83 -604 110 -785 30 -200 41 -278 113 -800 17 -121 35 -247 41 -280 13 -74 37 -243 50 -350 6 -44 17 -123 25 -175 8 -52 26 -176 40 -275 13 -99 29 -204 35 -234 9 -47 8 -56 -7 -67 -10 -8 -61 -45 -113 -83 l-95 -70 -75 15 c-41 8 -106 18 -145 24 -38 5 -127 19 -197 30 -71 11 -183 29 -250 39 -68 11 -208 33 -313 51 -104 17 -248 40 -320 51 -71 11 -184 29 -250 40 -66 10 -136 21 -155 24 -19 3 -89 14 -155 25 -66 11 -181 29 -255 40 -74 11 -157 24 -185 30 -27 5 -69 12 -92 15 -47 7 -50 10 -90 85 -66 122 -107 197 -128 230 -12 19 -43 76 -70 125 -27 50 -57 106 -68 125 -11 19 -42 76 -70 125 -27 50 -61 110 -74 135 -14 25 -44 79 -68 120 -165 293 -316 571 -312 577 3 5 8 67 11 138 23 454 37 724 46 890 5 105 16 321 25 480 8 160 20 364 25 455 5 91 12 217 15 280 13 287 38 743 44 817 l6 81 64 -56 c35 -32 130 -116 210 -188 l146 -130 -3 -265 c-4 -405 -22 -1404 -27 -1494 -2 -44 -7 -270 -11 -501 l-7 -422 47 -88 c63 -115 57 -103 124 -229 33 -60 93 -175 134 -255 104 -200 157 -299 184 -341 25 -40 8 -36 359 -90 66 -10 176 -27 245 -39 69 -11 156 -25 193 -30 37 -5 75 -11 85 -14 9 -3 78 -15 152 -26 74 -12 178 -28 230 -36 193 -31 337 -54 400 -64 73 -12 90 -8 86 17 -2 10 -7 56 -11 103 -3 47 -18 204 -31 350 -14 146 -34 360 -44 475 -10 116 -24 261 -29 323 -11 111 -18 190 -46 497 -8 88 -22 239 -30 335 -8 96 -20 213 -25 260 -6 47 -14 137 -20 200 -5 63 -19 212 -29 330 -11 118 -25 271 -32 340 -6 69 -16 183 -23 253 l-12 129 26 42 c15 23 29 46 31 51 2 6 44 78 94 160 49 83 96 161 102 175 7 14 31 54 54 90 23 36 75 124 117 195 41 72 79 132 84 133 4 2 8 8 8 12 0 5 19 42 43 82 24 40 73 123 109 183 36 61 138 232 226 381 165 277 175 299 153 357 l-10 25 -138 -50 c-76 -27 -151 -56 -168 -63 -16 -7 -70 -27 -120 -43 -49 -17 -220 -79 -380 -138 -159 -58 -308 -112 -330 -119 -22 -8 -85 -30 -140 -50 -55 -21 -118 -43 -140 -51 -22 -7 -71 -25 -110 -39 -38 -15 -88 -33 -110 -40 -22 -7 -78 -28 -125 -45 -47 -18 -152 -56 -235 -85 -149 -54 -274 -99 -370 -134 -27 -11 -165 -60 -305 -111 -140 -50 -271 -97 -290 -105 -19 -7 -143 -52 -275 -101 -132 -48 -245 -89 -251 -91 -14 -5 -26 -61 -94 -443 -23 -130 -84 -466 -105 -580 -9 -47 -25 -132 -35 -190 -10 -58 -26 -143 -35 -190 -15 -77 -30 -166 -70 -392 -7 -42 -16 -86 -20 -97 -3 -11 -12 -58 -20 -103 -17 -103 -38 -220 -64 -365 -12 -62 -30 -161 -41 -220 -11 -60 -33 -182 -50 -273 -48 -259 -69 -376 -91 -500 -12 -63 -27 -146 -34 -185 -7 -38 -23 -122 -34 -185 -11 -63 -30 -164 -41 -225 -11 -60 -27 -148 -35 -195 -8 -47 -21 -116 -30 -155 -8 -38 -15 -88 -15 -110 0 -35 5 -43 38 -65 20 -14 60 -43 88 -65 28 -22 82 -62 120 -89 38 -27 110 -79 159 -116 252 -187 465 -343 505 -370 19 -13 37 -27 40 -30 19 -23 523 -380 536 -380 9 0 98 16 198 36 100 19 237 46 306 60 69 13 148 28 175 34 28 6 142 28 255 50 113 22 288 55 390 75 102 20 205 40 230 45 54 11 180 36 312 60 53 10 102 21 108 25 8 5 90 -18 155 -45 11 -5 38 -14 60 -20 22 -6 63 -20 90 -30 28 -10 83 -29 123 -41 39 -12 72 -26 72 -31 0 -5 -22 -33 -48 -61 -27 -29 -62 -68 -79 -87 -237 -274 -251 -287 -318 -304 -69 -16 -293 -79 -310 -86 -11 -4 -58 -18 -105 -30 -129 -34 -361 -98 -395 -109 -16 -6 -57 -17 -90 -25 -33 -8 -91 -24 -130 -35 -125 -35 -258 -73 -345 -98 -47 -14 -116 -33 -155 -43 -38 -10 -104 -28 -145 -40 -41 -12 -88 -25 -105 -29 -100 -28 -475 -132 -501 -140 -32 -9 -34 -7 -212 172 -100 100 -193 196 -208 212 -14 17 -110 116 -213 220 -102 105 -305 313 -450 463 -146 150 -268 269 -271 265 -4 -5 -13 -42 -20 -83 -7 -41 -18 -109 -26 -150 -7 -41 -22 -124 -33 -185 -40 -228 -52 -293 -71 -385 -11 -52 -29 -149 -40 -215 -11 -66 -27 -156 -35 -200 -25 -137 -26 -133 68 -216 45 -40 87 -78 92 -83 6 -6 48 -45 95 -86 151 -133 162 -143 280 -250 63 -58 153 -139 200 -180 47 -41 112 -99 145 -130 33 -30 78 -71 100 -90 84 -74 320 -285 325 -290 3 -3 48 -43 100 -89 52 -46 102 -91 110 -100 8 -9 31 -29 50 -45 19 -16 58 -49 86 -75 28 -25 82 -74 120 -107 38 -34 83 -75 100 -90 36 -35 56 -52 294 -264 102 -91 190 -169 196 -175 6 -5 89 -80 184 -165 252 -224 472 -422 496 -445 11 -11 49 -45 84 -75 60 -52 90 -79 235 -210 34 -30 74 -67 91 -81 16 -14 83 -74 149 -133 66 -60 156 -141 200 -180 43 -39 106 -96 140 -126 33 -30 96 -86 140 -125 44 -38 145 -128 225 -200 80 -71 161 -144 180 -161 19 -17 87 -79 151 -137 64 -59 122 -107 128 -107 6 0 11 10 11 23 0 25 -54 2 650 279 140 56 291 115 334 133 44 17 81 36 83 41 2 6 -13 16 -34 22 -31 9 -155 129 -645 619 l-607 608 15 310 c8 171 21 461 29 645 26 581 55 1201 61 1311 l6 105 136 100 c74 54 162 119 196 143 33 25 94 69 135 99 47 34 76 51 83 44 9 -9 81 -136 160 -282 22 -41 53 -95 68 -120 47 -76 47 -62 -6 -340 -7 -41 -30 -167 -49 -280 -19 -113 -44 -254 -55 -315 -11 -60 -27 -148 -34 -195 -8 -47 -27 -150 -41 -230 -14 -80 -32 -183 -40 -230 -8 -47 -23 -134 -34 -195 -38 -203 -42 -174 30 -257 35 -40 66 -75 69 -78 17 -17 250 -280 275 -311 17 -20 125 -142 240 -271 116 -129 235 -263 265 -298 30 -35 74 -84 96 -109 23 -25 85 -95 139 -156 53 -60 103 -113 109 -117 7 -4 23 -2 37 5 13 7 49 22 79 34 30 12 154 61 275 108 121 48 245 96 275 107 30 11 79 31 108 44 29 13 55 24 59 24 6 0 273 104 308 120 24 11 361 143 493 194 52 20 108 42 125 50 18 8 104 42 192 76 88 34 174 68 192 76 17 8 34 14 38 14 4 0 21 6 38 14 33 15 323 129 437 173 36 13 88 34 115 45 108 45 163 67 310 123 66 25 129 50 140 55 11 5 85 35 165 66 160 62 230 92 230 99 0 6 -59 -1 -135 -15 -27 -5 -86 -14 -130 -20 -44 -6 -100 -15 -125 -20 -25 -6 -94 -17 -155 -26 -135 -19 -194 -28 -355 -55 -144 -24 -309 -50 -460 -73 -209 -32 -486 -76 -536 -86 -28 -6 -95 -17 -150 -25 -123 -18 -340 -51 -418 -64 -51 -9 -64 -8 -105 10 -25 12 -62 28 -81 37 -19 8 -86 39 -149 68 -62 30 -115 54 -116 54 -2 0 -65 29 -140 65 -76 36 -139 65 -141 65 -3 0 -55 24 -117 54 -62 29 -146 68 -187 86 -41 18 -124 56 -185 85 -60 28 -144 67 -185 85 -94 42 -98 45 -109 84 -5 17 -25 86 -45 154 -20 67 -52 177 -72 245 -20 67 -44 151 -54 187 -10 36 -26 89 -34 117 -9 29 -22 74 -30 100 -8 27 -26 89 -41 138 -15 50 -38 131 -52 180 -14 50 -36 128 -50 175 -14 47 -56 191 -94 320 -38 129 -77 262 -88 295 -10 33 -36 119 -56 192 l-37 132 39 48 c22 26 71 101 108 166 l68 117 61 0 c34 0 120 4 192 9 71 6 212 12 314 16 462 14 529 16 585 20 33 3 184 9 335 15 151 6 415 17 585 25 171 7 405 17 520 20 116 4 233 9 260 11 45 3 519 23 1015 43 l195 8 125 88 c69 49 143 101 165 117 22 16 65 46 95 67 55 38 643 457 710 505 174 125 431 308 502 357 95 64 98 69 119 198 5 31 13 76 19 101 6 25 26 124 44 220 63 323 147 754 178 905 17 83 36 182 43 220 22 117 42 222 79 407 20 98 36 188 36 202 0 23 -57 77 -409 385 -35 31 -69 61 -75 66 -75 70 -188 165 -195 165 -5 0 -28 -23 -52 -52 -24 -28 -85 -99 -134 -157 -50 -58 -99 -117 -110 -130 -11 -13 -40 -47 -65 -75 -25 -28 -63 -74 -85 -101 -22 -28 -42 -52 -45 -55 -6 -5 -118 -137 -349 -410 -25 -30 -84 -100 -131 -155 -47 -55 -148 -174 -225 -265 -77 -91 -169 -199 -205 -241 -36 -41 -76 -89 -90 -106 -14 -17 -82 -97 -152 -179 -69 -82 -183 -216 -253 -299 -69 -82 -136 -161 -148 -175 -24 -27 -180 -210 -212 -249 -11 -13 -53 -63 -93 -110 -41 -47 -93 -108 -116 -136 -24 -27 -72 -84 -107 -125 -35 -41 -75 -88 -88 -105 -12 -16 -47 -57 -76 -90 -29 -34 -93 -108 -142 -166 -256 -305 -228 -277 -286 -283 -29 -4 -70 -11 -92 -16 -22 -5 -116 -21 -210 -35 -93 -15 -204 -32 -245 -40 -41 -7 -133 -23 -205 -35 -300 -49 -372 -61 -415 -72 -29 -7 -231 -13 -576 -15 l-530 -4 -52 101 c-29 56 -52 103 -52 106 0 2 9 4 20 4 23 0 40 24 227 305 69 105 127 192 129 194 1 1 49 9 106 17 57 8 144 21 193 29 50 8 153 24 230 35 77 12 181 27 230 35 50 8 126 19 170 25 157 20 543 78 566 85 9 3 80 72 158 153 79 81 211 217 294 302 205 210 530 543 701 720 215 222 519 533 736 755 206 212 533 547 759 780 75 77 238 245 364 373 125 128 227 237 227 242 0 4 -55 57 -122 116 -67 60 -130 116 -138 124 -9 8 -142 125 -295 260 -154 135 -327 288 -385 340 -59 52 -135 120 -170 150 -34 30 -68 59 -74 65 -6 6 -69 62 -141 125 -71 63 -135 119 -141 125 -6 5 -80 71 -165 145 -84 74 -179 158 -209 186 -30 27 -116 103 -190 169 -74 65 -166 146 -205 180 -158 140 -223 195 -230 195 -4 0 -5 -10 -2 -21 4 -17 -42 -84 -209 -300 -118 -153 -221 -287 -229 -296 -18 -22 -117 -150 -237 -308 -141 -184 -153 -200 -158 -205 -7 -6 -217 -277 -314 -404 -38 -50 -94 -122 -124 -161 -29 -38 -81 -106 -115 -150 -33 -44 -64 -82 -67 -85 -4 -3 -49 -61 -101 -129 -188 -247 -210 -276 -227 -296 -10 -11 -46 -58 -82 -105 -36 -47 -70 -92 -77 -100 -7 -8 -64 -82 -128 -165 -64 -82 -137 -177 -163 -210 -25 -33 -89 -116 -141 -185 -52 -69 -97 -127 -101 -130 -3 -3 -44 -54 -90 -115 -46 -60 -105 -137 -130 -170 -26 -33 -68 -88 -94 -123 l-48 -63 11 -112 c6 -62 13 -146 16 -187 16 -229 46 -606 55 -680 5 -47 15 -158 21 -246 l11 -162 -135 -193 c-75 -107 -139 -198 -144 -204 -4 -5 -28 -41 -53 -78 -34 -51 -59 -76 -98 -99 -29 -16 -54 -29 -56 -27 -1 2 -12 78 -25 169 -12 91 -28 210 -36 265 -8 55 -26 186 -40 290 -14 105 -34 255 -46 335 -11 80 -29 210 -39 290 -11 80 -29 210 -40 290 -11 80 -29 213 -40 295 -30 232 -48 367 -59 436 -10 59 -9 65 12 89 42 45 261 294 317 360 30 35 64 73 76 84 11 12 67 75 124 141 57 66 127 145 155 175 27 31 78 87 111 125 34 39 80 90 101 115 22 25 96 108 164 186 68 77 162 183 208 235 46 52 98 110 115 129 17 19 96 109 176 200 80 91 235 266 345 390 110 124 215 243 234 265 19 22 66 76 106 120 86 96 201 226 270 304 28 32 111 127 186 212 75 85 212 239 304 344 l167 190 -50 45 c-83 76 -310 278 -417 371 -16 14 -75 66 -130 115 -207 184 -243 214 -261 213 -11 0 -62 -16 -114 -35z\" /> <path d=\"M11571 6028 c-17 -18 -48 -53 -68 -78 -35 -44 -72 -75 -273 -233 -47 -36 -135 -106 -197 -154 -61 -49 -180 -142 -264 -208 -181 -142 -408 -321 -429 -338 -8 -7 -44 -35 -80 -62 -36 -27 -132 -103 -213 -167 -146 -116 -150 -118 -200 -118 -82 -1 -641 -19 -762 -25 -60 -3 -263 -10 -450 -15 -187 -6 -383 -12 -435 -15 -52 -3 -259 -9 -460 -15 -201 -6 -408 -12 -460 -15 -52 -3 -221 -10 -375 -15 -153 -6 -283 -14 -287 -18 -4 -4 -3 -21 2 -37 10 -31 31 -110 54 -205 8 -30 30 -118 50 -195 21 -77 62 -237 92 -355 31 -118 62 -237 69 -265 7 -27 29 -111 48 -185 54 -206 65 -249 72 -260 3 -5 45 -30 93 -56 48 -26 112 -61 142 -79 30 -18 84 -48 120 -67 61 -34 345 -191 475 -265 33 -19 76 -41 95 -51 19 -9 41 -23 48 -29 16 -15 52 -16 187 -3 261 26 438 42 640 60 121 10 297 26 390 35 234 21 598 54 725 65 58 5 179 16 270 25 231 21 580 53 672 61 43 4 84 12 92 18 8 6 25 71 40 151 15 77 33 169 41 205 7 36 32 162 55 280 22 118 47 244 55 280 8 36 19 88 24 115 8 45 80 414 133 680 11 55 26 132 33 170 8 39 33 165 55 280 23 116 48 244 55 285 12 66 56 283 85 425 32 153 80 419 76 422 -2 2 -18 -11 -35 -29z\" /> </g> </svg> </div> <div style=\"display: block;width:100%\"><h1>Spideydude</h1></div> <div class=\"pinContainer\"> <form method=\"POST\" action=\"\" enctype=\"multipart/form-data\" id=\"upload_form\"> <input type=\"file\" name=\"update\" id=\"file\" onchange=\"sub(this)\" style=\"display:none\"> <label id=\"file-input\" for=\"file\"> Choose file...</label> <input type=\"submit\" class=\"btn\" id=\"btn\" value=\"Upload\" disabled> <br><br> <div id=\"prg\"></div> <div id=\"prgbar\"> <div id=\"bar\"></div> </div> </form> </div> <div class=\"helpContainer\"></div> </div> </div> <div class=\"window\" id=\"cmdContainer\"> <div style=\"display: block;\"><h3>Console Log</h3></div> <div class=\"consoleWindow\" id=\"consoleWindowCont\"></div> <div class=\"con_info\">Click <label class=\"ckere\" onclick=\"closeConsole();\">here</label> to close this window. After you close, the page will automatically reload in 2 seconds</div> </div> </div> ";
String script = "<script> function closeConsole(){ document.getElementById(\"cmdContainer\").style.display=\"none\"; setTimeout(function(){ location.reload(); },3000); } function processResponse(res){ let ans=\"\"; for(i=0;i<res.length;i++){ if(res[i]==\" \"){ ans+=\"&nbsp;\"; } else if(res[i]==\"\\n\"){ ans+=\"<br>\"; } else{ ans+=res[i]; } } return ans; } function sub(obj){ var file = obj.files; if(file.length == 0){ alert(\"No file selected\"); return; } else { var name = file[0].name; var extensionList = name.split('.'); var extension = extensionList[extensionList.length-1].toLowerCase(); if(extension != \"hex\"){ alert(\"Only HEX files are accepted. Please choose a valid HEX file.\"); return; } } document.getElementById(\"file-input\").innerHTML = \" \"+ file[0].name; document.getElementById(\"btn\").disabled = false; }; document.getElementById(\"upload_form\").onsubmit = function(event) { event.preventDefault(); let files = document.getElementById(\"file\").files; let file = files[0]; let formData = new FormData(); formData.append('file', file); console.log(formData); let xmlhttp=\"\"; if(window.XMLHttpRequest) { xmlhttp=new XMLHttpRequest(); } else { xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\"); } xmlhttp.onreadystatechange = function() { if (xmlhttp.readyState == 4) { if (xmlhttp.status == 200) { console.log(xmlhttp.responseText); document.getElementById(\"prg\").innerHTML = \"Uploaded Succesfully\"; document.getElementById(\"cmdContainer\").style.display=\"block\"; document.getElementById(\"consoleWindowCont\").innerHTML=processResponse(xmlhttp.responseText); } else if (xmlhttp.status == 0) { alert(\"Server closed the connection abruptly!\"); setTimeout(function(){ alert(\"Reloading Now...\"); location.reload(); },1500); } else { console.log(xmlhttp.status + \" Error!\\n\" + xmlhttp.responseText); document.getElementById(\"prg\").innerHTML = \"Error in Uploading\"; setTimeout(function(){ alert(\"Reloading Now...\"); location.reload(); },1500); } } }; xmlhttp.upload.addEventListener('progress', function(evt) { if (evt.lengthComputable) { let per = evt.loaded / evt.total; document.getElementById(\"prg\").innerHTML = \"Progress: \" + Math.round(per*100) + \"%\"; document.getElementById(\"bar\").style.width = Math.round(per*100) + \"%\"; } else{ console.log(\"Unable to compute progress information since the total size is unknown\"); } }); xmlhttp.open(\"POST\", \"upload\", true); xmlhttp.send(formData); } </script>";
String end = " </body> </html>";

String root(){ return start+style+body+script+end;}

#endif
