#ifndef OSMIUM_IO_HEADER_HPP
#define OSMIUM_IO_HEADER_HPP

/*

This file is part of Osmium (https://osmcode.org/libosmium).

Copyright 2013-2018 Jochen Topf <jochen@topf.org> and others (see README).

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

#include <osmium/osm/box.hpp>
#include <osmium/util/options.hpp>

#include <initializer_list>
#include <vector>

namespace osmium {

    namespace io {

        /**
         * Meta information from the header of an OSM file.
         *
         * The header can contain any number of bounding boxes, although
         * usually there is only a single one (or none). PBF files only
         * allow a single bounding box, but XML files can have multiple ones,
         * although it is unusual and the semantics are unclear, so it is
         * discouraged to create files with multiple bounding boxes.
         *
         * The header contains a flag telling you whether this file can
         * contain multiple versions of the same object. This is true for
         * history files and for change files, but not for normal OSM data
         * files. Not all OSM file formats can distinguish between those
         * cases, so the flag might be wrong.
         *
         * In addition the header can contain any number of key-value pairs
         * with additional information. Most often this is used to set the
         * "generator", the program that generated the file. Depending on
         * the file format some of these key-value pairs are handled
         * specially. See the Options parent class for details on how to
         * set and get those key-value pairs.
         */
        class Header : public osmium::Options {

            /// Bounding boxes
            std::vector<osmium::Box> m_boxes{};

            /**
             * Are there possibly multiple versions of the same object in
             * this stream of objects? This should be true for history files
             * and for change files, but not for normal OSM data files.
             */
            bool m_has_multiple_object_versions = false;

        public:

            Header() = default;

            Header(const std::initializer_list<osmium::Options::value_type>& values) :
                Options(values) {
            }

            /**
             * Get the bounding boxes defined in the header.
             */
            std::vector<osmium::Box>& boxes() noexcept {
                return m_boxes;
            }

            /**
             * Get the bounding boxes defined in the header.
             */
            const std::vector<osmium::Box>& boxes() const noexcept {
                return m_boxes;
            }

            /**
             * Set all the bounding boxes in the header.
             */
            Header& boxes(const std::vector<osmium::Box>& boxes) noexcept {
                m_boxes = boxes;
                return *this;
            }

            /**
             * Get the first (or only if there is only one) bounding box.
             *
             * Returns an empty, invalid box if there is none.
             */
            osmium::Box box() const {
                return m_boxes.empty() ? osmium::Box{} : m_boxes.front();
            }

            /**
             * Join up all the bounding boxes in the header into one and return
             * it. This method is what you probably want to use unless you want
             * to handle the possibly multiple bounding boxes yourself.
             *
             * Returns an empty, invalid box if there is none.
             */
            osmium::Box joined_boxes() const {
                osmium::Box box;
                for (const auto& b : m_boxes) {
                    box.extend(b);
                }
                return box;
            }

            /**
             * Add the given bounding box to the list of bounding boxes in the
             * header.
             *
             * @returns The header itself to allow chaining.
             */
            Header& add_box(const osmium::Box& box) {
                m_boxes.push_back(box);
                return *this;
            }

            /**
             * Can this file contain multiple versions of the same object?
             */
            bool has_multiple_object_versions() const noexcept {
                return m_has_multiple_object_versions;
            }

            /**
             * Set the flag that tells us whether this file can contain
             * multiple versions of the same object?
             *
             * @returns The header itself to allow chaining.
             */
            Header& set_has_multiple_object_versions(bool value) noexcept {
                m_has_multiple_object_versions = value;
                return *this;
            }

        }; // class Header

    } // namespace io

} // namespace osmium

#endif // OSMIUM_IO_HEADER_HPP
