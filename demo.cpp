/*
 * demo.cpp
 *
 *  Created on: 3 Sep 2018
 *      Author: simon
 */

#include "uart.h"
#include "misc_asm.h"
#include "inter_process.h"
#include "common.h"

#define FP_SCALE_SHIFT 6

class fixed_point
{
public:
	fixed_point(void);
	fixed_point(const fixed_point &a);

	fixed_point(unsigned int a);
	inline operator fixed_point();

	void set_value_by_unsigned_int(unsigned int a)
	{
		value = a << FP_SCALE_SHIFT;
	}

	short value;
};


inline fixed_point::fixed_point(void)
{
	value = 0;
}
inline fixed_point::fixed_point(const fixed_point &a)
{
	value = a.value;
}

inline fixed_point::fixed_point(unsigned int a)
{
	set_value_by_unsigned_int(a);
}

inline fixed_point operator+(fixed_point a, fixed_point b)
{
	fixed_point f;
	f.value = a.value + b.value;
	return f;
}

inline fixed_point operator-(fixed_point a, fixed_point b)
{
	fixed_point f;
	f.value = a.value - b.value;
	return f;
}

inline bool operator<(fixed_point a, fixed_point b)
{
	return a.value < b.value;
}

inline bool operator<=(fixed_point a, fixed_point b)
{
	return a.value <= b.value;
}

static bool eval(fixed_point coord_x, fixed_point coord_y, fixed_point res_x, fixed_point res_y, short width_shift, short height_shift)
{
	coord_x.value -= (34 << FP_SCALE_SHIFT);

    fixed_point col = coord_x;
    fixed_point row = coord_y;
    fixed_point width = res_x;
    fixed_point height = res_y;

	fixed_point half_width, half_height;
	half_width.value = width.value >> 1;
	half_height.value = height.value >> 1;

	fixed_point part_x = col - half_width;
	fixed_point part_y = row - half_height;

	fixed_point c_re, c_im;
	c_re.value = part_x.value >> (width_shift - 1);
	c_im.value = part_y.value >> (height_shift - 1);

    fixed_point x;
	fixed_point y;

    const short maximum = 64;
	short last_it = maximum;

    for (short iteration = maximum - 1; iteration >= 0; iteration--)
	{
    	trap0(DEBUGGER_UPDATE);

		fixed_point v;
		fixed_point part_x, part_y;
		part_x.value = x.value * x.value;
		part_y.value = y.value * y.value;

		v.value = (part_x.value + part_y.value) >> FP_SCALE_SHIFT;
		if (v.value <= (4 << FP_SCALE_SHIFT))
        {
			fixed_point x_new_temp, x_new;
			x_new_temp.value = (part_x.value - part_y.value) >> FP_SCALE_SHIFT;
			x_new = x_new_temp + c_re;

			fixed_point temp_y;
			temp_y.value = (x.value * y.value) >> (FP_SCALE_SHIFT - 1);
			y = temp_y + c_im;
            x = x_new;

            last_it = iteration;
        }
		else
			break;
	}

    if (last_it == 0)
        return true;
    else
        return false;
}

static char output[128 * 256];

void mandel(void)
{
	const short width_shift = 7;
	const short height_shift = 8;
	fixed_point width(1U << width_shift), height(1U << height_shift);

	char *p = output;

	for (fixed_point y; y < height; y.value += (1 << FP_SCALE_SHIFT))
	{
		for (fixed_point x; x < width; x.value += (1 << FP_SCALE_SHIFT))
		{
			bool e = eval(x, y, width, height, width_shift, height_shift);

			if (e)
			{
				*p++ = 'x';
				put_char_user('x');
			}
			else
			{
				*p++ = ' ';
				put_char_user(' ');
			}
		}
		put_char_user('\n');
		*p++ = '\n';
	}
}

void delay(void)
{
	for (volatile unsigned int i = 0; i < 1000; i++);
}

#ifdef HAS_PCI
void pci_io_write8(unsigned int addr, unsigned char v, unsigned int offset)
{
	volatile unsigned char *p = (unsigned char *)(PCI_IO | PCI_IO_PASSTHROUGH(offset & 3) | addr);
	*p = v;
}

unsigned char pci_io_read8(unsigned int addr, unsigned int offset)
{
	volatile unsigned char *p = (unsigned char *)(PCI_IO | PCI_IO_PASSTHROUGH(offset & 3) | addr);
	return *p;
}

void pci_io_write16(unsigned int addr, unsigned short v, unsigned int offset)
{
	volatile unsigned short *p = (unsigned short *)(PCI_IO | PCI_IO_PASSTHROUGH(offset & 3) | addr);
	*p = v;
}

unsigned short pci_io_read16(unsigned int addr, unsigned int offset)
{
	volatile unsigned short *p = (unsigned short *)(PCI_IO | PCI_IO_PASSTHROUGH(offset & 3) | addr);
	return *p;
}

unsigned char pci_mem_read8(unsigned int addr)
{
	volatile unsigned char *p = (unsigned char *)(PCI_MEM | addr);
	return *p;
}

unsigned short pci_mem_read16(unsigned int addr)
{
	volatile unsigned short *p = (unsigned short *)(PCI_MEM | addr);
	return *p;
}

void pci_print(void)
{
	/*volatile unsigned int *p = (unsigned int *)PCI_CONFIG;

	for (unsigned int count = 0; count < 16; count++)
	{
		for (int i = 0; i < 8; i++)
		{
			unsigned int v = p[count];

			put_hex_num_user(count * 4);
			put_char_user(' ');

			put_hex_byte_user(v & 0xff);
			put_hex_byte_user((v >> 8) & 0xff);

			put_char_user(' ');

			put_hex_byte_user((v >> 16) & 0xff);
			put_hex_byte_user(v >> 24);

			put_char_user('\n');

			delay();
		}
		put_char_user('\n');
	}
*/

	volatile unsigned short *p = (unsigned short *)PCI_CONFIG;

	for (unsigned int count = 0; count < 16; count++)
	{
		for (int i = 0; i < 3; i++)
		{
			unsigned short v = p[count * 2 + 1];

			put_hex_num_user(count * 4);
			put_char_user(' ');

			put_hex_byte_user(v & 0xff);
			put_hex_byte_user((v >> 8) & 0xff);

			put_char_user(' ');

			v = p[count * 2];

			put_hex_byte_user(v & 0xff);
			put_hex_byte_user(v >> 8);

			put_char_user('\n');

			delay();
		}
		put_char_user('\n');
	}
}

#endif

extern "C" void _start(void)
{
	put_string_user("in loaded user image\n");
//	mandel();

#ifdef HAS_PCI
	volatile bool config_write_go = false;
	volatile unsigned int *p = (unsigned int *)PCI_CONFIG;

	volatile bool io_write16_go = false;
	volatile bool io_read16_go = false;

	volatile bool io_write8_go = false;
	volatile bool io_read8_go = false;

	volatile bool mem_read8_go = false;
	volatile bool mem_read16_go = false;

	volatile unsigned int offset = 0;
	volatile unsigned int offset_io = 0;
	volatile unsigned int to_write = 0;

	volatile unsigned int loop_count = 1;
	volatile unsigned int steps = 1;


	volatile bool print_out = false;

	while(1)
	{
		if (config_write_go)
		{
			config_write_go = false;
			p[offset >> 2] = to_write;
		}

		if (print_out)
		{
			pci_print();
			print_out = false;
		}

		if (io_write16_go)
		{
			for (unsigned int count = 0; count < loop_count; count++)
				pci_io_write16(offset, to_write, offset_io);
			io_write16_go = false;
		}

		if (io_read16_go)
			for (unsigned int count = 0; count < loop_count; count++)
			{
				unsigned short c = pci_io_read16(offset, offset_io);
				io_read16_go = false;

				put_hex_byte_user(c >> 8);
				put_hex_byte_user(c & 0xff);
				put_char_user('\n');
			}

		if (io_write8_go)
		{
			for (unsigned int count = 0; count < loop_count; count++)
				pci_io_write8(offset, to_write, offset_io);
			io_write8_go = false;
		}

		if (io_read8_go)
			for (unsigned int count = 0; count < loop_count; count++)
			{
				unsigned char c = pci_io_read8(offset, offset_io);
				io_read8_go = false;

				put_hex_byte_user(c);
				put_char_user('\n');
			}

		if (mem_read8_go)
			for (unsigned int its = 0; its < steps; its++)
			{
				for (unsigned int count = 0; count < loop_count; count++)
				{
					unsigned char c = pci_mem_read8(offset + its);
					mem_read8_go = false;

					put_hex_byte_user(c);
					put_char_user(' ');
				}

				put_char_user('\n');
			}

		if (mem_read16_go)
			for (unsigned int its = 0; its < steps; its++)
			{
				for (unsigned int count = 0; count < loop_count; count++)
				{
					unsigned short c = pci_mem_read16(offset + its * 2);
					mem_read16_go = false;

					put_hex_byte_user(c >> 8);
					put_hex_byte_user(c & 0xff);
					put_char_user(' ');
				}

				put_char_user('\n');
			}

		trap0(DEBUGGER_UPDATE);
	}
#endif

	while (1)
		trap0(DEBUGGER_UPDATE);
}

