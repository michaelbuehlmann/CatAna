//
// Created by Michael Bühlmann on 08/12/15.
//

#ifndef CATANA_APP_FIELD_HELPER_HPP
#define CATANA_APP_FIELD_HELPER_HPP

#include <fftw3.h>
#include <boost/iterator/iterator_facade.hpp>
#include <cassert>
#include <tuple>

namespace catana {

    //! FFTW Field Class with storage management.
    template<class T>
    struct fftw_field {
    public:
        T* data;
        size_t size;

    public:
        //! Constructor:
        fftw_field()
                :size(0), data(nullptr) { }

        fftw_field(size_t size)
                :size(size)
        {
            data = (T*) fftw_malloc(sizeof(T)*size);
        }

        //! Destructor:
        ~fftw_field()
        {
            fftw_free(data);
        }

        //! Movable
        fftw_field(fftw_field&& other)
        {
            size = other.size;
            other.size = 0;
            data = other.data;
            other.data = nullptr;
        }

        //! Move-Assignable
        fftw_field& operator=(fftw_field&& other)
        {
            std::swap(data, other.data);
            std::swap(size, other.size);
            return *this;
        }

        //! Copyable
        fftw_field(fftw_field& other)
                :fftw_field(other.size)
        {
            std::copy(other.data, other.data+size, data);
        }

        //! Assignable
        fftw_field& operator=(const fftw_field& other)
        {
            fftw_field cpy(other);
            std::swap(*this, cpy);
            return *this;
        }
    };

//! Field Frequencies
/*
 *  For a 3D Field, along each axis frequencies are assigned [0,1,...M/2-1 , -M/2,...,-1]
 */
    struct freq {
        int f1;
        int f2;
        int f3;
    };

//! Wrapper around FFTW Field with iterator, which accounts for padded fields
    template<class T>
    struct field_wrapper {
        //! Default constructor
        field_wrapper()
                :data(nullptr), M(0), M3(0), padded_field(false) { }

        //! Constructor from existing array (MxMxM)
        field_wrapper(T* data, size_t M, const bool padded_field = false)
                :data(data), M(M), M3(M), padded_field(padded_field) { }

        //! Constructor from existing array (MxMxM3)
        field_wrapper(T* data, size_t M, size_t M3, const bool padded_field = false)
                :data(data), M(M), M3(M3), padded_field(padded_field) { }

        T* data;
        size_t M, M3;
        bool padded_field;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // ITERATOR CLASS //////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template<class Value>
        class iter : public boost::iterator_facade<
                field_wrapper::iter<Value>, Value, boost::random_access_traversal_tag
        > {
        public:
            using difference_type = ptrdiff_t;

            iter()
                    :begin(nullptr), pos(0), M(0), M3(0), padded_field(false) { }

            explicit iter(T* begin, size_t pos, size_t M, bool inline_field = false)
                    :begin(begin), pos(pos), M(M), M3(M), padded_field(inline_field) { }

            explicit iter(T* begin, size_t pos, size_t M, size_t M3, bool inline_field = false)
                    :begin(begin), pos(pos), M(M), M3(M3), padded_field(inline_field) { }

            //! Return frequencies for each axis, [0, 1, ..., M/2-1, -M/2, -M/2+1, ..., -1]
            freq get_frequency() const
            {
                size_t k = pos%(M*M3);
                size_t j = (pos/M3)%M;
                size_t i = pos/(M3*M);

                freq f;
                f.f1 = static_cast<int>(i<M/2 ? i : i-(M+1));
                f.f2 = static_cast<int>(j<M/2 ? j : j-(M+1));
                f.f3 = static_cast<int>(k<M/2 ? k : k-(M+1));
                return f;
            }

        private:
            friend class boost::iterator_core_access;

            T* begin;
            size_t pos;
            size_t M, M3;
            bool padded_field;

            bool equal(iter<Value> const& other) const
            {
                return begin==other.begin &&
                        pos==other.pos &&
                        M==other.M &&
                        M3==other.M3 &&
                        padded_field==other.padded_field;
            }

            T& dereference() const
            {
                if (padded_field) {
                    return *(begin+pos+2*(pos/M3));
                }
                else {
                    return *(begin+pos);
                }
            }

            void increment()
            {
                assert(pos!=M*M*M3);
                ++pos;
            }

            void decrement()
            {
                assert(pos!=0);
                --pos;
            }

            void advance(difference_type n)
            {
                pos += n;
                assert(pos<M*M*M3);
            }

            difference_type distance_to(iter<Value> const& other)
            {
                if (pos>=other.pos) {
                    return pos-other.pos;
                }
                else {
                    return other.pos-pos;
                }
            }
        };
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        using iterator = iter<T>;
        using const_iterator = iter<T const>;

        iterator begin()
        {
            return iterator(data, 0, M, M3, padded_field);
        }

        iterator end()
        {
            return iterator(data, M*M*M3, M, M3, padded_field);
        }
    };

}
#endif //CATANA_APP_FIELD_HELPER_HPP