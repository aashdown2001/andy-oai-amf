/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file
 \brief
 \author
 \date 2021
 \email: contact@openairinterface.org
 */

#include "ConfigurationUpdateCommand.hpp"

#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
ConfigurationUpdateCommand::ConfigurationUpdateCommand() {
  plain_header = NULL;
  // TODO
}

//------------------------------------------------------------------------------
ConfigurationUpdateCommand::~ConfigurationUpdateCommand() {}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type, CONFIGURATION_UPDATE_COMMAND);
}

//------------------------------------------------------------------------------
int ConfigurationUpdateCommand::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding ConfigurationUpdateCommand message");
  int encoded_size = 0;
  if (!plain_header) {
    Logger::nas_mm().error("Mandatory IE missing Header");
    return 0;
  }

  Logger::nas_mm().debug(
      "Encoded ConfigurationUpdateCommand message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int ConfigurationUpdateCommand::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding ConfigurationUpdateCommand message");
  int decoded_size = 3;
  plain_header     = header;

  Logger::nas_mm().debug(
      "Decoded ConfigurationUpdateCommand message len (%d)", decoded_size);
  return 1;
}
