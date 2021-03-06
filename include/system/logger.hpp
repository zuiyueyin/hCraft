/* 
 * hCraft - A custom Minecraft server.
 * Copyright (C) 2012-2013	Jacob Zhitomirsky (BizarreCake)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _hCraft__LOGGER_H_
#define _hCraft__LOGGER_H_

#include <ostream>
#include <sstream>
#include <pthread.h> // used for thread-local storage.
#include <mutex>
#include <fstream>


namespace hCraft {
	
	/* 
	 * An enumeration of all possible types of messages that can be recorded by
	 * the logger, *roughly* sorted (but not completely) in ascending order of
	 * importance.
	 */
	enum logtype
	{
		LT_DEBUG,  // not logged to file.
		LT_SYSTEM,
		
		LT_CHAT,
		LT_IRC,
		LT_DRAW,
		LT_CONSOLE,
		
		LT_INFO,   // usually information about an error.
		LT_WARNING,
		LT_ERROR,
		LT_FATAL,
	};
	
	
	/* 
	 * The logger provides an interface through which clients can record
	 * information about happened events to the console, and if enabled,
	 * to a file on disk in a thread-safe manner whilst still exploiting
	 * the power and type-safety of I/O streams.
	 */
	class logger
	{
		friend class logger_buf;
		class logger_buf: public std::stringbuf
		{
			logger& log;
			
		public:
			/* 
			 * Class constructor.
			 */
			logger_buf (logger& log);
			
			
			/* 
			 * Locks the buffer's underlying mutex and outputs whatever's in the
			 * internal string buffer out to the standard output stream.
			 */
			virtual int sync ();
		};
		
	public:
		class logger_strm: public std::ostream
		{
			logger_buf buf;
			
		public:
			/* 
			 * Class constructor.
			 */
			logger_strm (logger& log);
		};
		
	private:
		std::mutex lock;
		pthread_key_t strm_key; // a key to per-thread instances of `logger_strm'.
		
		std::ofstream fs;
		int fsc;
		int day;
		
	private:
		/* 
		 * Reopens the internal file stream and sets its path to a file whose name
		 * contains the appropriate date.
		 */
		void recalc_date ();
		
	public:
		/* 
		 * Class constructor.
		 * 
		 * Throws `std::runtime_error' on failure.
		 */
		logger ();
		
		
		/* 
		 * Returns a stream unique to the calling thread that it can use to log
		 * events. Returning a thread-local instance this way ensures
		 * thread-safety.
		 * 
		 * NOTE: After messages are written to the stream, the stream MUST be
		 * flushed (through any means, i.e: std::endl, std::flush, or just by
		 * directly calling flush () on it). Also note that by flushing the
		 * logger, the actual underlying stream is not necessarily flushed as
		 * well, this is just to let the logger know that the message has been
		 * fully written.
		 */
		logger_strm& operator() (logtype lt = LT_SYSTEM);
	};
}

#endif

