// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file SerCon.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _SERCON_H_
#define _SERCON_H_

// TODO Poner en el contexto.

#include <stdint.h>
#include <array>
#include <string>
#include <vector>
#include <map>
#include <bitset>

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif
#else
#define eProsima_user_DllExport
#endif

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(SerCon_SOURCE)
#define SerCon_DllAPI __declspec( dllexport )
#else
#define SerCon_DllAPI __declspec( dllimport )
#endif // SerCon_SOURCE
#else
#define SerCon_DllAPI
#endif
#else
#define SerCon_DllAPI
#endif // _WIN32

namespace eprosima
{
    namespace fastcdr
    {
        class Cdr;
    }
}


/*!
 * @brief This class represents the structure SerCon defined by the user in the IDL file.
 * @ingroup SERCON
 */
class SerCon
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport SerCon();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~SerCon();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object SerCon that will be copied.
     */
    eProsima_user_DllExport SerCon(const SerCon &x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object SerCon that will be copied.
     */
    eProsima_user_DllExport SerCon(SerCon &&x);

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object SerCon that will be copied.
     */
    eProsima_user_DllExport SerCon& operator=(const SerCon &x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object SerCon that will be copied.
     */
    eProsima_user_DllExport SerCon& operator=(SerCon &&x);

    /*!
     * @brief This function sets a value in member seq
     * @param _seq New value for member seq
     */
    eProsima_user_DllExport void seq(uint64_t _seq);

    /*!
     * @brief This function returns the value of member seq
     * @return Value of member seq
     */
    eProsima_user_DllExport uint64_t seq() const;

    /*!
     * @brief This function returns a reference to member seq
     * @return Reference to member seq
     */
    eProsima_user_DllExport uint64_t& seq();

    /*!
     * @brief This function copies the value in member com
     * @param _com New value to be copied in member com
     */
    eProsima_user_DllExport void com(const std::string &_com);

    /*!
     * @brief This function moves the value in member com
     * @param _com New value to be moved in member com
     */
    eProsima_user_DllExport void com(std::string &&_com);

    /*!
     * @brief This function returns a constant reference to member com
     * @return Constant reference to member com
     */
    eProsima_user_DllExport const std::string& com() const;

    /*!
     * @brief This function returns a reference to member com
     * @return Reference to member com
     */
    eProsima_user_DllExport std::string& com();

    /*!
     * @brief This function returns the maximum serialized size of an object
     * depending on the buffer alignment.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    eProsima_user_DllExport static size_t getMaxCdrSerializedSize(size_t current_alignment = 0);

    /*!
     * @brief This function returns the serialized size of a data depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Serialized size.
     */
    eProsima_user_DllExport static size_t getCdrSerializedSize(const SerCon& data, size_t current_alignment = 0);


    /*!
     * @brief This function serializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    eProsima_user_DllExport void serialize(eprosima::fastcdr::Cdr &cdr) const;

    /*!
     * @brief This function deserializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    eProsima_user_DllExport void deserialize(eprosima::fastcdr::Cdr &cdr);



    /*!
     * @brief This function returns the maximum serialized size of the Key of an object
     * depending on the buffer alignment.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    eProsima_user_DllExport static size_t getKeyMaxCdrSerializedSize(size_t current_alignment = 0);

    /*!
     * @brief This function tells you if the Key has been defined for this type
     */
    eProsima_user_DllExport static bool isKeyDefined();

    /*!
     * @brief This function serializes the key members of an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    eProsima_user_DllExport void serializeKey(eprosima::fastcdr::Cdr &cdr) const;

private:
    uint64_t m_seq;
    std::string m_com;
};

#endif // _SERCON_H_