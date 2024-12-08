概要
********
mruby/cを用いて、Zephyrサンプルコード「Blinky」相当の動作を実現する

mruby/c取得方法
********
#. cd mrubyc
#. git submodule init
#. git submodule update

動作確認環境
********
* nRF52832
* nRF52840
* nRF9151
* nRF54L15

ライセンス
********
* /src/lib/mrubyc/hal.c, /src/lib/mrubyc/hal.hは「BSD-3-Clause license」に拠る（mruby/c由来のコード）
* /CMakeLists.txt, /src/main.cは「Apache-2.0」に拠る（Zephyrサンプルコード由来のコード）

参考情報
********
* https://www.s-itoc.jp/support/technical-support/mrubyc/
* https://academy.nordicsemi.com/
* https://devzone.nordicsemi.com/
* https://docs.nordicsemi.com/
* https://docs.zephyrproject.org/latest/
* https://github.com/zephyrproject-rtos/zephyr/tree/main/samples
