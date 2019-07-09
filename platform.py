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
from platform import system

from platformio.managers.platform import PlatformBase


class ShaktiPlatform(PlatformBase):

    def get_boards(self, id_=None):
        result = PlatformBase.get_boards(self, id_)
        if not result:
            return result
        if id_:
            return self._add_default_debug_tools(result)
        else:
            for key, value in result.items():
                result[key] = self._add_default_debug_tools(result[key])
        return result

    def _add_default_debug_tools(self, board):
        debug = board.manifest.get("debug", {})
        upload_protocols = board.manifest.get("upload", {}).get(
            "protocols", [])
        if "tools" not in debug:
            debug['tools'] = {}

        tools = ("ftdi", "jlink", "qemu")
        for link in tools:
            if link not in upload_protocols or link in debug['tools']:
                continue
            if link == "jlink":
                assert debug.get("jlink_device"), (
                    "Missed J-Link Device ID for %s" % board.id)
                debug['tools'][link] = {
                    "server": {
                        "package": "tool-jlink",
                        "arguments": [
                            "-singlerun",
                            "-if", "JTAG",
                            "-select", "USB",
                            "-speed", "1000",
                            "-jtagconf", "-1,-1",
                            "-device", debug.get("jlink_device"),
                            "-port", "2331"
                        ],
                        "executable": ("JLinkGDBServerCL.exe"
                                       if system() == "Windows" else
                                       "JLinkGDBServer")
                    },
                    "onboard": link in debug.get("onboard_tools", [])
                }

            elif link == "qemu":
                machine64bit = "64" in board.get("build.mabi")
                debug['tools'][link] = {
                    "server": {
                        "package": "tool-qemu-riscv",
                        "arguments": [
                            "-nographic",
                            "-machine", "sifive_%s" % (
                                "u" if machine64bit else "e"),
                            "-d", "unimp,guest_errors",
                            "-gdb", "tcp::1234",
                            "-S"
                        ],
                        "executable": "bin/qemu-system-riscv%s" % (
                            "64" if machine64bit else "32")
                    },
                    "onboard": True
                }
            else:
                server_args = [
                    "-s", "$PACKAGE_DIR/share/openocd/scripts",
                    "-f", join(
                        self.get_package_dir("framework-shakti-sdk"),
                        "bsp", "third_party", board.id, "spike.cfg")
                ]
                debug['tools'][link] = {
                    "server": {
                        "package": "tool-openocd-riscv",
                        "executable": "bin/openocd",
                        "arguments": server_args
                    },
                    "onboard": link in debug.get("onboard_tools", []),
                    "init_cmds": debug.get("init_cmds", None)
                }

        board.manifest['debug'] = debug
        return board
