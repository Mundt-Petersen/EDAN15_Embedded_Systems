/* ex16.lp
   example 16.10 in Lee&Seshia book for EDAN15
   (with any errors existent in the book)
*/

/* Looking for WCET. BB time is ~size in bytes for this particular case */
max: 27 x1 + 2 x2 + 10 x3 + 16 x4 + 31 x5 + 4 x6;

x1 = 1;
x6 = 1;
x1 = d12;
x2 = d12 + d52;
x2 = d23 + d26;
x3 = d23;
x3 = d45; /* this does not seem right? */
x4 = d34;
x4 = d45;
x5 = d35 + d45;
x5 = d52;
x6 = d26;

x3 <= 32;

int x1,x2,x3,x4,x5,x6;