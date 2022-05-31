# Copyright 2014-present PlatformIO <contact@platformio.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from os.path import join

from platformio.public import PlatformBase


class ShaktiPlatform(PlatformBase):

    def get_boards(self, id_=None):
        result = super().get_boards(id_)
        if not result:
            return result
        if id_:
            return self._add_dynamic_options(result)
        else:
            for key in result:
                result[key] = self._add_dynamic_options(result[key])
        return result

    def _add_dynamic_options(self, board):
        debug = board.manifest.get("debug", {})
        debug_tools = ("jlink", "ftdi")

        upload_protocol = board.manifest.get("upload", {}).get("protocol")
        upload_protocols = board.manifest.get("upload", {}).get("protocols", [])
        upload_protocols.extend(debug_tools)
        if upload_protocol and upload_protocol not in upload_protocols:
            upload_protocols.append(upload_protocol)
        board.manifest['upload']['protocols'] = upload_protocols

        if "tools" not in debug:
            debug['tools'] = {}

        for link in upload_protocols:
            server_args = [
                "-f",
                join(
                    self.get_package_dir("framework-shakti-sdk") or "", "bsp",
                    "third_party", board.id, "%s.cfg" % link)
            ]

            debug['tools'][link] = {
                "server": {
                    "package": "tool-openocd-riscv",
                    "executable": "bin/openocd",
                    "arguments": server_args
                },
                "onboard": link in debug.get("onboard_tools", [])
            }

        board.manifest['debug'] = debug
        return board
