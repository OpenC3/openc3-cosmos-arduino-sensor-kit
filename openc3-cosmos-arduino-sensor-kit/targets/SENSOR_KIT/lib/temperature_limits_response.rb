# encoding: ascii-8bit

# Copyright 2022 Ball Aerospace & Technologies Corp.
# All Rights Reserved.
#
# This program is free software; you can modify and/or redistribute it
# under the terms of the GNU Affero General Public License
# as published by the Free Software Foundation; version 3 with
# attribution addendums as found in the LICENSE.txt
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.

# Modified by OpenC3, Inc.
# All changes Copyright 2024, OpenC3, Inc.
# All Rights Reserved
#
# This file may also be used under the terms of a commercial license
# if purchased from OpenC3, Inc.

# This file implements a class to handle responses to limits state changes.

require 'openc3/packets/limits_response'

class TemperatureLimitsResponse < OpenC3::LimitsResponse
  def call(packet, item, old_limits_state)
    case item.limits.state
    when :RED_HIGH
      cmd("SENSOR_KIT TRIGGER_BUZZER with SYNC 17, ID 2, STATE 1")
    when :YELLOW_HIGH
      cmd("SENSOR_KIT SET_LED with SYNC 17, ID 1, STATE 1")
    
    when :GREEN_HIGH
      cmd("SENSOR_KIT TRIGGER_BUZZER with SYNC 17, ID 2, STATE 0")
      cmd("SENSOR_KIT SET_LED with SYNC 17, ID 1, STATE 0")

    when :GREEN_LOW
      cmd("SENSOR_KIT TRIGGER_BUZZER with SYNC 17, ID 2, STATE 0")
      cmd("SENSOR_KIT SET_LED with SYNC 17, ID 1, STATE 0")

    end
  end
end
