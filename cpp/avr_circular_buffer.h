/* Circular Buffer, an object oriented circular buffer (AVR version).
 * Copyright (C) 2015-2020 Enrico Rossi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _AVR_CBUFFER_H_
#define _AVR_CBUFFER_H_

#include <stdlib.h>

#ifndef CBUF_SIZE // Default buffer size
#define CBUF_SIZE 16
#endif

/*! Buffer structure

 [ | | | | | | | | | | | | | | | | | | | | | | | | ]
  ^buffer   ^start                        ^idx    ^TOP
            ^------------ len ------------^
  ^---------------------- size -------------------^
 */

// CBuffer of D objects indexed by T type.
template <typename T, typename D>
class CBuffer {
	private:
		// Fixed array size
		D* buffer_;
		T idx_;
		T start_;
		bool overflow_;
		T len_; // byte left in the buffer
		virtual void pop_object(D*);
		virtual void push_object(D);
	protected:
		T TOP_;
		const T size_;
		void set_start(T s) { start_ = s; };
		void set_len(T l) { len_ = l; };
	public:
		// debugging methods
		T size() const { return size_; };
		T len() const { return len_; };
		bool overflow() const { return overflow_; };
		T index() const { return idx_; };
		T start() const { return start_; };
		// FIXME i < size_
		T operator[](T const i) const { return buffer_[i]; };
		// contructor
		CBuffer(T size = CBUF_SIZE);
		~CBuffer() { free(buffer_); }; // Destructor
		virtual void clear();
		bool popc(D*);
		T pop(D*, const T);
		T popm(D*, const T, const D);
		bool push(D);
};

//! Clear the buffer.
template <typename T, typename D>
void CBuffer<T, D>::clear()
{
	idx_ = 0;
	start_ = 0;
	len_ = 0;
	overflow_ = false;
}

/*! Initialize the buffer.
 *
 * \param plugin enable the check_eom function plugin.
 * \return the allocated struct.
 */
template <typename T, typename D>
CBuffer<T, D>::CBuffer(T sz) : size_{sz}
{
	buffer_ = (D*) malloc(sizeof(D) * size_);
	TOP_ = size_ - 1;
	clear();
}

/*! Get single object from the buffer.
 *
 * This perform the operation, which can be overridden
 * depending on the type of object.
 */
template <typename T, typename D>
void CBuffer<T, D>::pop_object(D *data)
{
	*data = buffer_[start_];
}

/*! Extract a single object from the buffer.
 *
 * data = buffer[start]
 *
 * \param data the area where to copy the object.
 * \return true if ok
 * \warning possible race condition!
 */
template <typename T, typename D>
bool CBuffer<T, D>::popc(D *data)
{
	if (len_) {
		pop_object(data); // override me

		if (start_ == TOP_)
			start_ = 0;
		else
			start_++;

		// From here possible race condition with push().
		len_--;
		overflow_ = false;

		return (true);
	} else {
		return (false);
	}
}

/*! Pop everything present in the buffer.
 *
 * start_ to the current idx_.
 *
 * \param data the area where to copy the message if found.
 * \param sizeofdata.
 * \return the number of bytes fetched.
 * \warning if the data size is less than the buffer, only the sizeofdata
 * byte get fetched, the buffer remain not empty.
 * \warning if the sizeofdata is bigger than the allocated data,
 * it will segfault or worse.
 */
template <typename T, typename D>
T CBuffer<T, D>::pop(D* data, const T sizeofdata)
{
	T j {0};

	// while there is space left on data and byte in the buffer
	while ((j < sizeofdata) && popc(data + j))
		j++;

	return (j);
}

/*! Pop everything from start_ to EOM.
 *
 * If no EOM is found then all the content of the buffer
 * is copied like pop().
 *
 * If the size of data is less then the message in the buffer, then
 * data get filled and the rest of the message is left in the buffer.
 *
 * \param data the area where to copy the message.
 * \param sizeofdata.
 * \param eom the EndOfMessage.
 * \return the number of byte copied.
 *
 * \note EOM is NOT copied.
 * \warning race condition!
 */
template <typename T, typename D>
T CBuffer<T, D>::popm(D* data, const T sizeofdata, const D eom)
{
	T j {0};

	// while there is space left on data
	//  and byte in the buffer
	//  and the data fetched is NOT EOM
	while ((j < sizeofdata) && popc(data + j) && (*(data + j) != eom))
		j++;

	return (j);
}

/*! Push the single object.
 *
 * This function should be overridden.
 */
template <typename T, typename D>
void CBuffer<T, D>::push_object(D c)
{
	buffer_[idx_] = c;
}

/*! add data to the buffer.
 *
 * \note if overflow and EOM then the last char must be the EOM.
 *
 * \warning race condition with other functions.
 *  modified members data: overflow_, idx_, buffer_[idx_], len_
 */
template <typename T, typename D>
bool CBuffer<T, D>::push(D c)
{
	// If the buffer is full do nothing.
	if (overflow_) {
		return (false);
	} else {
		// catch overflow
		if (start_) {
			// idx next to start?
			if (idx_ == (start_ - 1))
				overflow_ = true;
		} else {
			// idx next to the end of the buffer?
			if (idx_ == TOP_)
				overflow_ = true;
		}

		push_object(c); // override me

		if (idx_ == TOP_)
			idx_ = 0;
		else
			idx_++;

		len_++;
		return (true);
	}
}

#endif
