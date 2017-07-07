/******************************************************************************
 * $Id: ocpnnmea2000_pi.h,v 1.0 2011/02/26 01:54:37 nohal Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPNNMEA2000 Plugin
 * Author:   Manuel Bouyer
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Manuel Bouyer
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#ifndef NMEA2000_FRAME_H_
#define NMEA2000_FRAME_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include <sys/socket.h>
#ifdef __NetBSD__
#include <netcan/can.h>
#else
#include <linux/can.h>
#include <linux/can/raw.h>
#endif


class nmea2000_frame {
    public:
	inline nmea2000_frame() {init();}
	inline nmea2000_frame(struct can_frame *f)
	    { frame  = f; data = f->data;}
	inline ~nmea2000_frame() {};
	inline bool is_pdu1() const
	    { return (((frame->can_id >> 16) & 0xff) < 240); };
	    
	inline int getsrc() const { return (frame->can_id & 0xff); };
	inline int getdst() const
	    { return (is_pdu1() ? (frame->can_id >> 8) & 0xff : -1); };
	inline int getpgn() const
	    { return (is_pdu1() ? (frame->can_id >> 8) & 0x1ff00 : (frame->can_id >> 8) & 0x1ffff); }

	inline int getpri() const { return ((frame->can_id >> 26) & 0x7); };
	inline int getlen() const { return (frame->can_dlc); };
	inline const unsigned char *getdata() const {return (data); };
	inline ssize_t readframe(int s) {
	    return read(s, frame, sizeof(struct can_frame));
	}

	inline int8_t frame2int8(int i) const
	    {return (data[i]);}
	inline int16_t frame2int16(int i) const
	    {return ((int16_t)data[i+1] << 8) | data[i];}
	inline int32_t frame2int32(int i) const
	    {return ((int32_t)data[i+3] << 24) |
	            ((int32_t)data[i+2] << 16) |
	            ((int32_t)data[i+1] << 8) |
		    data[i];
	     }
	inline uint8_t frame2uint8(int i) const
	    {return (data[i]);}
	inline uint16_t frame2uint16(int i) const
	    {return ((uint16_t)data[i+1] << 8) | data[i];}
	inline uint32_t frame2uint24(int i) const
	    {return ((uint32_t)data[i+2] << 16) |
	            ((uint32_t)data[i+1] << 8) |
		    data[i];
	    }
	inline uint32_t frame2uint32(int i) const
	    {return ((uint32_t)data[i+3] << 24) |
	            ((uint32_t)data[i+2] << 16) |
	            ((uint32_t)data[i+1] << 8) |
		    data[i];
	    }
	inline void int82frame(int8_t v, int i)
	    {data[i] = v;}
	inline void int162frame(int16_t v, int i)
	    {data[i+1] = (v >> 8) & 0xff; data[i] = v & 0xff;}
	inline void int322frame(int32_t v, int i)
	    {
		data[i+3] = (v >> 24) & 0xff;
		data[i+2] = (v >> 16) & 0xff;
		data[i+1] = (v >>  8) & 0xff;
		data[i] = v & 0xff;
	    }
	inline void uint82frame(u_int8_t v, int i)
	    {data[i] = v;}
	inline void uint162frame(u_int16_t v, int i)
	    {data[i+1] = (v >> 8) & 0xff; data[i] = v & 0xff;}
	inline void uint242frame(u_int32_t v, int i)
	    {
		data[i+2] = (v >> 16) & 0xff;
		data[i+1] = (v >>  8) & 0xff;
		data[i] = v & 0xff;
	    }
	inline void uint322frame(u_int32_t v, int i)
	    {
		data[i+3] = (v >> 24) & 0xff;
		data[i+2] = (v >> 16) & 0xff;
		data[i+1] = (v >>  8) & 0xff;
		data[i] = v & 0xff;
	    }
	    
    protected:
	struct can_frame *frame;
	uint8_t *data;
    private:
	struct can_frame _frame;
	inline void init() 
	    { frame = &_frame;
	      memset(frame, 0, sizeof(struct can_frame));
	      data = frame->data;
	    }
};

#endif