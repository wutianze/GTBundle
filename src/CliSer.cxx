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
 * @file CliSer.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace { char dummy; }
#endif

#include "CliSer.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

CliSer::CliSer()
{
    // m_seq com.eprosima.idl.parser.typecode.PrimitiveTypeCode@74ad1f1f
    m_seq = 0;
    // m_com com.eprosima.idl.parser.typecode.StringTypeCode@69b0fd6f
    m_com ="";

}

CliSer::~CliSer()
{


}

CliSer::CliSer(const CliSer &x)
{
    m_seq = x.m_seq;
    m_com = x.m_com;
}

CliSer::CliSer(CliSer &&x)
{
    m_seq = x.m_seq;
    m_com = std::move(x.m_com);
}

CliSer& CliSer::operator=(const CliSer &x)
{

    m_seq = x.m_seq;
    m_com = x.m_com;

    return *this;
}

CliSer& CliSer::operator=(CliSer &&x)
{

    m_seq = x.m_seq;
    m_com = std::move(x.m_com);

    return *this;
}

size_t CliSer::getMaxCdrSerializedSize(size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + 512 + 1;


    return current_alignment - initial_alignment;
}

size_t CliSer::getCdrSerializedSize(const CliSer& data, size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + data.com().size() + 1;


    return current_alignment - initial_alignment;
}

void CliSer::serialize(eprosima::fastcdr::Cdr &scdr) const
{

    scdr << m_seq;
    scdr << m_com;
}

void CliSer::deserialize(eprosima::fastcdr::Cdr &dcdr)
{

    dcdr >> m_seq;
    dcdr >> m_com;
}

/*!
 * @brief This function sets a value in member seq
 * @param _seq New value for member seq
 */
void CliSer::seq(uint64_t _seq)
{
m_seq = _seq;
}

/*!
 * @brief This function returns the value of member seq
 * @return Value of member seq
 */
uint64_t CliSer::seq() const
{
    return m_seq;
}

/*!
 * @brief This function returns a reference to member seq
 * @return Reference to member seq
 */
uint64_t& CliSer::seq()
{
    return m_seq;
}

/*!
 * @brief This function copies the value in member com
 * @param _com New value to be copied in member com
 */
void CliSer::com(const std::string &_com)
{
m_com = _com;
}

/*!
 * @brief This function moves the value in member com
 * @param _com New value to be moved in member com
 */
void CliSer::com(std::string &&_com)
{
m_com = std::move(_com);
}

/*!
 * @brief This function returns a constant reference to member com
 * @return Constant reference to member com
 */
const std::string& CliSer::com() const
{
    return m_com;
}

/*!
 * @brief This function returns a reference to member com
 * @return Reference to member com
 */
std::string& CliSer::com()
{
    return m_com;
}

size_t CliSer::getKeyMaxCdrSerializedSize(size_t current_alignment)
{
    size_t current_align = current_alignment;





    return current_align;
}

bool CliSer::isKeyDefined()
{
   return false;
}

void CliSer::serializeKey(eprosima::fastcdr::Cdr &scdr) const
{
    (void) scdr;
     
     
}
