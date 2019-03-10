#ifndef OSMIUM_OSM_OBJECT_COMPARISONS_HPP
#define OSMIUM_OSM_OBJECT_COMPARISONS_HPP

/*

This file is part of Osmium (https://osmcode.org/libosmium).

Copyright 2013-2019 Jochen Topf <jochen@topf.org> and others (see README).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <osmium/osm/object.hpp>
#include <osmium/osm/timestamp.hpp>
#include <osmium/util/misc.hpp>

#include <cassert>
#include <cstdlib>
#include <tuple>

namespace osmium {

    /**
     * Function object class for comparing OSM objects for equality by type,
     * ID, and version.
     */
    struct object_equal_type_id_version {

        bool operator()(const osmium::OSMObject& lhs, const osmium::OSMObject& rhs) const noexcept {
            return lhs == rhs;
        }

        /// @pre lhs and rhs must not be nullptr
        bool operator()(const osmium::OSMObject* lhs, const osmium::OSMObject* rhs) const noexcept {
            assert(lhs && rhs);
            return *lhs == *rhs;
        }

    }; // struct object_equal_type_id_version

    /**
     * Function object class for comparing OSM objects for equality by type
     * and ID, ignoring the version.
     */
    struct object_equal_type_id {

        bool operator()(const osmium::OSMObject& lhs, const osmium::OSMObject& rhs) const noexcept {
            return lhs.type() == rhs.type() &&
                   lhs.id() == rhs.id();
        }

        /// @pre lhs and rhs must not be nullptr
        bool operator()(const osmium::OSMObject* lhs, const osmium::OSMObject* rhs) const noexcept {
            assert(lhs && rhs);
            return operator()(*lhs, *rhs);
        }

    }; // struct object_equal_type_id

    /**
     * Compare two objects IDs. Order is as follows: 0 first, then negative
     * IDs, then positive IDs, both ordered by their absolute values.
     */
    struct id_order {

        bool operator()(const object_id_type lhs, const object_id_type rhs) const noexcept {
            return const_tie(lhs > 0, std::abs(lhs)) <
                   const_tie(rhs > 0, std::abs(rhs));
        }

    }; // struct id_order

    /**
     * Function object class for ordering OSM objects by type, id, version,
     * and timestamp.
     */
    struct object_order_type_id_version {

        bool operator()(const osmium::OSMObject& lhs, const osmium::OSMObject& rhs) const noexcept {
            return lhs < rhs;
        }

        /// @pre lhs and rhs must not be nullptr
        bool operator()(const osmium::OSMObject* lhs, const osmium::OSMObject* rhs) const noexcept {
            assert(lhs && rhs);
            return *lhs < *rhs;
        }

    }; // struct object_order_type_id_version

    /**
     * Function object class for ordering OSM objects by type, id, and version.
     *
     * The naming is a bit awkward here, but necessary to keep backwards
     * compatibility with object_order_type_id_version.
     */
    struct object_order_type_id_version_without_timestamp {

        bool operator()(const osmium::OSMObject& lhs, const osmium::OSMObject& rhs) const noexcept {
            return const_tie(lhs.type(), lhs.id() > 0, lhs.positive_id(), lhs.version()) <
                   const_tie(rhs.type(), rhs.id() > 0, rhs.positive_id(), rhs.version());
        }

        /// @pre lhs and rhs must not be nullptr
        bool operator()(const osmium::OSMObject* lhs, const osmium::OSMObject* rhs) const noexcept {
            assert(lhs && rhs);
            return operator()(*lhs, *rhs);
        }

    }; // struct object_order_type_id_version_without_timestamp

    /**
     * Function object class for ordering OSM objects by type, ID, and
     * reverse version, timestamp. So objects are ordered by type and ID
     * (negative IDs first, then positive IDs, both in the order of their
     * absolute values), but later versions of an object are ordered before
     * earlier versions of the same object. This is useful when the last
     * version of an object needs to be used.
     */
    struct object_order_type_id_reverse_version {

        bool operator()(const osmium::OSMObject& lhs, const osmium::OSMObject& rhs) const noexcept {
            return const_tie(lhs.type(), lhs.id() > 0, lhs.positive_id(), rhs.version(),
                        ((lhs.timestamp().valid() && rhs.timestamp().valid()) ? rhs.timestamp() : osmium::Timestamp())) <
                   const_tie(rhs.type(), rhs.id() > 0, rhs.positive_id(), lhs.version(),
                        ((lhs.timestamp().valid() && rhs.timestamp().valid()) ? lhs.timestamp() : osmium::Timestamp()));
        }

        /// @pre lhs and rhs must not be nullptr
        bool operator()(const osmium::OSMObject* lhs, const osmium::OSMObject* rhs) const noexcept {
            assert(lhs && rhs);
            return operator()(*lhs, *rhs);
        }

    }; // struct object_order_type_id_reverse_version

} // namespace osmium

#endif // OSMIUM_OSM_OBJECT_COMPARISONS_HPP
