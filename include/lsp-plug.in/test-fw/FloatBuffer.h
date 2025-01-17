/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 21 авг. 2018 г.
 *
 * lsp-test-fw is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-test-fw is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-test-fw. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LSP_PLUG_IN_TEST_FW_FLOATBUFFER_H_
#define LSP_PLUG_IN_TEST_FW_FLOATBUFFER_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/test-fw/types.h>
#include <lsp-plug.in/test-fw/helpers.h>

#include <stdio.h>

namespace lsp
{
    namespace test
    {
        /**
         * Represents a float buffer with specific memory alignment
         * and constant boundary markers that helps to detect buffer
         * overruns/underruns
         */
        class FloatBuffer
        {
            private:
                float      *pBuffer;
                uint8_t    *pData;
                size_t      nLength;
                size_t      nAlign;
                bool        bAligned;
                mutable ssize_t  nLastDiff;

            private:
                void allocate(size_t samples, size_t align, bool aligned);

            public:
                explicit FloatBuffer(size_t samples, size_t align = DEFAULT_ALIGN, bool aligned = true);
                explicit FloatBuffer(const FloatBuffer &src);
                ~FloatBuffer();

            public:
                inline void randomize_positive()            { ::lsp::test::randomize_positive(pBuffer, nLength); };
                inline void randomize_negative()            { ::lsp::test::randomize_negative(pBuffer, nLength); };
                inline void randomize_sign()                { ::lsp::test::randomize_sign(pBuffer, nLength); };
                inline void randomize_0to1()                { ::lsp::test::randomize_0to1(pBuffer, nLength); };
                inline void randomize(float min, float max) { ::lsp::test::randomize(pBuffer, nLength, min, max); }
                void randomize()                            { ::lsp::test::randomize_positive(pBuffer, nLength); };
                void fill_zero();
                void fill(float v);
                void copy(const FloatBuffer &buf);
                void copy(const float *buf, size_t count);
                void replace(const float *buf, size_t count);
                void vfill(size_t off, size_t n, ...);

                bool validate() const;
                inline bool valid() const { return validate(); };
                inline bool corrupted() const { return !validate(); };
                bool equals_relative(const FloatBuffer &src, float tolerance = 1e-5f) const;
                bool equals_absolute(const FloatBuffer &src, float tolerance = 1e-5f) const;
                bool equals_adaptive(const FloatBuffer &src, float tolerance = 1e-5f) const;

                void dump(FILE *out, const char *text) const;
                void dump(FILE *out, const char *text, size_t from, size_t count) const;
                inline void dump(const char *text) const { dump(stdout, text); }
                inline void dump(const char *text, size_t from, size_t count) const { dump(stdout, text, from, count); }

                void dump_hex(FILE *out, const char *text) const;
                void dump_hex(FILE *out, const char *text, size_t from, size_t count) const;
                void dump_hex(const char *text) const { dump(stdout, text); }
                void dump_hex(const char *text, size_t from, size_t count) const { dump(stdout, text, from, count); }

                inline bool aligned() const         { return bAligned; }
                inline size_t size() const          { return nLength; }
                inline ssize_t last_diff() const    { return nLastDiff; }
                inline float get_diff() const       { return (nLastDiff >= 0) ? pBuffer[nLastDiff] : 0.0f; }

            public:
                inline float &operator [] (int index) {
                    return pBuffer[index];
                }

                inline float operator [] (int index) const {
                    return pBuffer[index];
                }

                inline float &operator [] (size_t index) {
                    return pBuffer[index];
                }

                inline float operator [] (size_t index) const {
                    return pBuffer[index];
                }

                inline float get(size_t index) const {
                    return pBuffer[index];
                }

                template <typename T>
                    inline T *data()
                    {
                        return reinterpret_cast<T *>(pBuffer);
                    }

                inline float *data()
                {
                    return pBuffer;
                }

                inline float *data(size_t offset)
                {
                    return &pBuffer[offset];
                }

                inline operator float *()
                {
                    return pBuffer;
                }

                inline operator const float *() const
                {
                    return pBuffer;
                }

        };
    }
}

#endif /* LSP_PLUG_IN_TEST_FW_FLOATBUFFER_H_ */
