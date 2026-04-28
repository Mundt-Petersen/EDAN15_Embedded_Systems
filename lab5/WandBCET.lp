/* max for WCET and min for BCET, with weights: 2 x1 + 2 x2 + 2 x3 + 1 x4 + 30 x5 + 1 x6 + 1 x7 + 30 x8 + 2 x9 + 1 x10 */
max: x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9 + x10;

x1 = 1;
x10 = 1;
x1 = d12;
x2 = d12 + d92;
x2 = d210 + d23;
x3 = d23;
x3 = d34 + d36;
x4 = d34;
x4 = d45 + d46;
x5 = d45;
x5 = d56;
x6 = d34 + d45 + d56;
x6 = d67 + d69;
x7 = d67;
x7 = d78 + d79;
x8 = d78;
x8 = d89;
x9 = d69 + d79 + d89;
x9 = d92;
x10 = d210;

x2 >= 5;
x2 <= 40;

int x1, x2, x3, x4, x5, x6, x7, x8, x9, x10;