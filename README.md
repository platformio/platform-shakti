# Shakti: development platform for [PlatformIO](http://platformio.org)

[![Build Status](https://github.com/platformio/platform-shakti/workflows/Examples/badge.svg)](https://github.com/platformio/platform-shakti/actions)

Shakti is an open-source initiative by the RISE group at IIT-Madras, which is not only building open source, production grade processors, but also associated components like interconnect fabrics, verification tools, storage controllers, peripheral IPs and SOC tools.

* [Home](https://registry.platformio.org/platforms/platformio/shakti) (home page in the PlatformIO Registry)
* [Documentation](https://docs.platformio.org/page/platforms/shakti.html) (advanced usage, packages, boards, frameworks, etc.)

# Usage

1. [Install PlatformIO](http://platformio.org)
2. Create PlatformIO project and configure a platform option in [platformio.ini](https://docs.platformio.org/page/projectconf.html) file:

## Stable version

```ini
[env:stable]
platform = shakti
board = ...
...
```

## Development version

```ini
[env:development]
platform = https://github.com/platformio/platform-shakti.git
board = ...
...
```

# Configuration

Please navigate to [documentation](https://docs.platformio.org/page/platforms/shakti.html).
