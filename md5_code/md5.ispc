

const unsigned int k[64] =
{
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

// r specifies the per-round shift amounts
const unsigned int r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

// leftrotate function definition
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))


inline void
to_bytes( varying int val, varying unsigned int8 * uniform bytes)
{
    bytes[0] = (unsigned int8) val;
    bytes[1] = (unsigned int8) (val >> 8);
    bytes[2] = (unsigned int8) (val >> 16);
    bytes[3] = (unsigned int8) (val >> 24);
}


inline void
to_bytes_varying(varying unsigned int val0,
                  varying unsigned int val1,
                  varying unsigned int val2,
                  varying unsigned int val3,
                  varying unsigned int &bytes0,
                  varying unsigned int &bytes1,
                  varying unsigned int &bytes2,
                  varying unsigned int &bytes3)
{
    bytes0 =  val0;
    bytes1 =  val1;
    bytes2 =  val2;
    bytes3 =  val3;
}


export void
md5_cal(uniform unsigned int8 initial_msgs[][5],
        uniform int new_len,
        uniform int total_num,
        uniform unsigned int8 we_want_find[16], uniform unsigned int8 final[5])
{

// Initialize vari  ables - simple count in nibbles:
foreach(j = 0 ... total_num){
    // These vars will contain the hash
    unsigned int  h0 = 0x67452301;
    unsigned int  h1 = 0xefcdab89;
    unsigned int  h2 = 0x98badcfe;
    unsigned int  h3 = 0x10325476;

    unsigned int w[16];
    unsigned int a, b, c, d, i, f, g, temp;
    unsigned int8 msg[64];
    //Pre-processing:
    //append "1" bit to message
    //append "0" bits until message length in bits ≡ 448 (mod 512)
    //append length mod (2^64) to message

    int char_num =0;
    for(int q=0;q<5;q++)
    {
      if(initial_msgs[j][q]!= 0)
        {
        char_num += 1;
        msg[q] = initial_msgs[j][q];
      }
    }

    msg[char_num] = 0x80; // append the "1" bit; most significant bit is "first"

    for (int offset = char_num + 1; offset < new_len; offset++)
        msg[offset] = 0; // append "0" bits

    // append the len in bits at the end of the buffer.
    to_bytes(char_num*8, msg + new_len);
    // initial_len[i]>>29 == initial_len[i]*8>>32, but avoids overflow.
    to_bytes(char_num>>29, msg + new_len + 4);

    // Process the message in successive 512-bit chunks:
    //for each 512-bit chunk of message:
    for(int offset = 0; offset < new_len; offset += (512/8)) {

        // break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
        for (i = 0; i < 16; i++)
            w[i] = (unsigned int) (msg + offset + i*4)[0] |
            ((unsigned int) (msg + offset + i*4)[1] << 8) |
            ((unsigned int) (msg + offset + i*4)[2] << 16)|
            ((unsigned int) (msg + offset + i*4)[3] << 24);

        // Initialize hash value for this chunk:
        a = h0;
        b = h1;
        c = h2;
        d = h3;

        // Main loop:
        for(i = 0; i<64; i++) {

            if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

            temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;

        }

        // Add this chunk's hash to result so far:
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;

    }

    if(
        we_want_find[0] == (unsigned int8) h0 &&
        we_want_find[1] == (unsigned int8) (h0 >> 8)&&
        we_want_find[2] == (unsigned int8) (h0 >> 16)&&
        we_want_find[3] == (unsigned int8) (h0 >> 24)&&
        we_want_find[4] == (unsigned int8) h1&&
        we_want_find[5] == (unsigned int8) (h1 >> 8)&&
        we_want_find[6] == (unsigned int8) (h1 >> 16)&&
        we_want_find[7] == (unsigned int8) (h1 >> 24)&&
        we_want_find[8] == (unsigned int8) h2&&
        we_want_find[9] == (unsigned int8) (h2 >> 8)&&
        we_want_find[10] == (unsigned int8) (h2 >> 16)&&
        we_want_find[11] == (unsigned int8) (h2 >> 24)&&
        we_want_find[12] == (unsigned int8) h3&&
        we_want_find[13] == (unsigned int8) (h3 >> 8)&&
        we_want_find[14] == (unsigned int8) (h3 >> 16)&&
        we_want_find[15] == (unsigned int8) (h3 >> 24)
        )
    {
      for(int l=0;l<5;l++)
        {
          final[l] = initial_msgs[j][l];
        }
    }
  }
}

//MD5 Task function
task void
md5_task(uniform unsigned int8 initial_msgs[][5],
        uniform int new_len,
        uniform int len_of_each_task,
        uniform int total_data,
        uniform unsigned int8 we_want_find[16], uniform unsigned int8 final[5]){
        // Initialize vari  ables - simple count in nibbles:
        uniform int start = taskIndex * len_of_each_task;
  	    uniform int end = min((taskIndex+1) * len_of_each_task, (unsigned int)total_data);

        foreach(j = start ... end){
            // These vars will contain the hash
            unsigned int  h0 = 0x67452301;
            unsigned int  h1 = 0xefcdab89;
            unsigned int  h2 = 0x98badcfe;
            unsigned int  h3 = 0x10325476;

            unsigned int w[16];
            unsigned int a, b, c, d, i, f, g, temp;
            unsigned int8 msg[64];
            //Pre-processing:
            //append "1" bit to message
            //append "0" bits until message length in bits ≡ 448 (mod 512)
            //append length mod (2^64) to message
            int char_num =0;
            for(int q=0;q<5;q++)
            {
              if(initial_msgs[j][q]!= 0)
                {
                char_num += 1;
                msg[q] = initial_msgs[j][q];
              }
            }

            msg[char_num] = 0x80; // append the "1" bit; most significant bit is "first"

            for (int offset = char_num + 1; offset < new_len; offset++)
                msg[offset] = 0; // append "0" bits

            // append the len in bits at the end of the buffer.
            to_bytes(char_num*8, msg + new_len);
            // initial_len[i]>>29 == initial_len[i]*8>>32, but avoids overflow.
            to_bytes(char_num>>29, msg + new_len + 4);

            // Process the message in successive 512-bit chunks:
            //for each 512-bit chunk of message:int8
            for(int offset = 0; offset < new_len; offset += (512/8)) {

                // break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
                for (i = 0; i < 16; i++)
                    w[i] = (unsigned int) (msg + offset + i*4)[0] |
                    ((unsigned int) (msg + offset + i*4)[1] << 8) |
                    ((unsigned int) (msg + offset + i*4)[2] << 16)|
                    ((unsigned int) (msg + offset + i*4)[3] << 24);

                // Initialize hash value for this chunk:
                a = h0;
                b = h1;
                c = h2;
                d = h3;

                // Main loop:
                for(i = 0; i<64; i++) {

                    if (i < 16) {
                        f = (b & c) | ((~b) & d);
                        g = i;
                    } else if (i < 32) {
                        f = (d & b) | ((~d) & c);
                        g = (5*i + 1) % 16;
                    } else if (i < 48) {
                        f = b ^ c ^ d;
                        g = (3*i + 5) % 16;
                    } else {
                        f = c ^ (b | (~d));
                        g = (7*i) % 16;
                    }

                    temp = d;
                    d = c;
                    c = b;
                    b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
                    a = temp;

                }

                // Add this chunk's hash to result so far:
                h0 += a;
                h1 += b;
                h2 += c;
                h3 += d;

            }

          if(we_want_find[0] == (unsigned int8) h0 &&
              we_want_find[1] == (unsigned int8) (h0 >> 8)&&
              we_want_find[2] == (unsigned int8) (h0 >> 16)&&
              we_want_find[3] == (unsigned int8) (h0 >> 24)&&
              we_want_find[4] == (unsigned int8) h1&&
              we_want_find[5] == (unsigned int8) (h1 >> 8)&&
              we_want_find[6] == (unsigned int8) (h1 >> 16)&&
              we_want_find[7] == (unsigned int8) (h1 >> 24)&&
              we_want_find[8] == (unsigned int8) h2&&
              we_want_find[9] == (unsigned int8) (h2 >> 8)&&
              we_want_find[10] == (unsigned int8) (h2 >> 16)&&
              we_want_find[11] == (unsigned int8) (h2 >> 24)&&
              we_want_find[12] == (unsigned int8) h3&&
              we_want_find[13] == (unsigned int8) (h3 >> 8)&&
              we_want_find[14] == (unsigned int8) (h3 >> 16)&&
              we_want_find[15] == (unsigned int8) (h3 >> 24)
              )
          {
            for(int l=0;l<5;l++)
              {
                final[l] = initial_msgs[j][l];
              }
          }
	}
}

// A task wrapper for md5_tast
export void
md5_task_wrap(uniform unsigned int8 initial_msgs[][5],
            uniform int new_len,
            uniform int total_num,
            uniform unsigned int8 we_want_find[],
            uniform int number_of_threads,
            uniform unsigned int8 final[]){
		uniform int len_of_data = total_num / number_of_threads;
		launch[number_of_threads] md5_task(initial_msgs,
            new_len,
            len_of_data,
            total_num,
            we_want_find,
            final);

}
