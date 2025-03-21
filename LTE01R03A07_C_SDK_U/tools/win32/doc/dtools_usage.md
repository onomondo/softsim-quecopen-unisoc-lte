>本文档基于windows下的dtools.exe进行编写。

# remote tools
> 这一部分的命令，都需要连接板子进行操作，连接mode可从以下根据需要8选1

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    --cmm <cmm>                  cmm file for offline mode
    --mips                       mips offline mode
> 需要选择elf文件的命令，应保证选择的elf文件与板子中烧录的pac文件版本一致

## bsdump
* blue screen dump

>该命令用以dump蓝屏数据，需在chip panic状态下才可使用该命令，根据不同芯片从对应的芯片文件夹下读取配置，如8850则从setting/chip/8850/8850_bsdump.xml中读取相关配置，最终生成cmm文件，以供后续死机分析。该命令对应dtools_win.exe中的blue screen dump功能。

Options:

    --profile <profile>          profile in XML file
>在setting/bgdump.xml文件中支持的profile，可用--profile list列出所有支持的profile项

Arguments:

    output                       output directory

For example:

    $ dtools.exe bsdump --profile list
      supported profiles:
         8910DM, 8811, 8850
    $ dtools.exe bsdump --diagport COM43,2000000 ./outputdir/bs --profile 8811
## memdump
* memory dump

>该命令用以dump memory指定address与size的数据，并存放于指定的output路径文件中。该命令对应dtools_win.exe中的memory dump功能。

Options:

    --lod                        write lod file rather than bin
    //最终输出lod而非bin文件

Arguments:

    address                      memory starting address
    size                         memory size
    output                       output dump file
For example:

    $ dtools.exe memdump 0x00000 512 ./outputdir/memdump.bin  --diagport COM43,2000000
## memwrite
* memory write

Arguments:

    type                         memory write type(w32/w16/w8)
    address                      memory write address
    value                        memory write value
For example:

    $ dtools.exe memwrite w8 0x0 0x55 --diagport COM43,2000000
      write ok
## memrecdump
* heap malloc/free record dump

>该命令用以查看内存申请/释放记录，并输出到output路径下，需要指定与chip烧录的同版本elf文件的存放路径。该命令对应dtools_win.exe中的misc dump中的heap malloc/free records (memory records)功能。
Arguments:

    output                       output file name
    elf                          elf file name
For example:

    $ dtools.exe memrecdump --debuguart COM51,921600 \
                ./outputdir/memrec ./pac/8910DM_cat1_v2.elf
## pmdump
* power management information dump
>该命令用以dump电源管理相关信息。该命令对应dtools_win.exe中的misc dump中的power managements功能。

Options:

    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
>此命令只支持live连接模式，因此连接方式5选1

Arguments:

    elf                          elf file name
    output                       output file name
For example:

    $ dtools.exe pmdump --debuguart COM51,921600 ./pac/8910DM_cat1_v2.elf ./outputdir/pmdump
      timer dump
      nbtimer dump
      sxtimer dump
      PM source dump
      clock constraint dump

## heapreport
* heap analysis report

>该命令用以dump堆栈信息。该命令对应dtools_win.exe中的heap report功能。

Options:

    --type <type>                heap report type

Arguments:

    output                       output file name
    elf                          elf file name

For example:

    $ dtools.exe heapreport --type list
      supported types:
         8910AP, 8910CP, 8811, 8850AP, 8850CP
    $ dtools.exe heapreport --type 8910AP --debuguart COM51,921600 \
                ./outputdir/heapreport ./pac/8910DM_cat1_v2.elf

## profile
* profile dump

Options:

    --ignore-jump                ignore tick jump
    --dump-raw                   dump raw events

Arguments:

    elf                          elf file name
    output                       output file name

For example:

    $ dtools.exe profile ./pac/8910DM_cat1_v2.elf ./outputdir/profile --debuguart COM51,921600
      find symbol gProfileCtx at 0x80c21c0c
      Profile buffer: 0x80c15c0c
      Profile size:   12288
      Profile start:  2562
      Profile count:  12288
      cannot find symbol g_spalProfileControl
      spal profile will not dump
      cannot find symbol gRfdigProfileCtxPtr
      RFDIG/RISCV profile will not dump
      //具体profile文件存储在设定的outputdir文件夹路径下

## heapinfo
* heap usage info dump

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6 选 1；但是目前只支持8811选择diagport mode

Arguments:

    output                       output file name
For example:

    $ dtools.exe heapinfo ./outputdir/heapinfo --diagport COM43,2000000
## irqinfo
* IRQ info dump

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6 选 1；但是目前只支持8811选择diagport mode
Arguments:

    output                       output file name
For example:

    $ dtools.exe irqinfo ./outputdir/irqinfo --diagport COM43,2000000

## readefuse
* read efuse word

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6选1

Arguments:

    id                           efuse id, bit 15 for double bit. When not
                                 specified, read all efuse words.

## writeefuse
* write efuse word

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6选1

Arguments:

    id                           efuse id, bit 15 for double bit
    val                          value to be written
For example:

    $ dtools.exe writeefuse id value --diagport COM43,2000000

## readnv
* read nv

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6选1

Arguments:

    nvid                         nvid
    output                       output file name
For example:

    $ dtools.exe readnv 620 ./outputdir/nv --diagport COM43,2000000
      //nvid需填写对应id

## writenv
* write nv

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6选1

Arguments:

    nvid                         nvid
    input                       input file name
For example:

    $ dtools.exe writenv 620 ./outputdir/nv --diagport COM43,2000000
      //nvid需填写对应id
# remote system command
>remote system command这一部分的命令，都需要连接板子进行操作，且chip处于live状态。


## ping
* ping remote target

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6选1

Arguments:

    ping

For example：
    $ dtools.exe ping --debughost COM51,921600

## panic
* force panic

>该命令是chip操作。该命令对应dtools_win.exe中的命令行panic功能，即对应菜单栏“chip->Force panic”功能。

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6选1

Arguments:

    panic

For example:

    $ dtools.exe panic --debughost COM51,921600
      //此处发送数据为包头+FD 02 00 00

## reboot
* normal reboot

>该命令是chip操作，重启chip。该命令对应dtools_win.exe中的命令行chip功能，即对应菜单栏“chip->Reboot”功能。

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6选1

Arguments:

    reboot
For example:

    $ dtools.exe reboot --debughost COM51,921600
      //此处发送数据为包头+FD 03 00 00

## rebootdnld
* reboot to download mode

>该命令是chip操作，重启chip并进入下载模式。该命令对应dtools_win.exe中的命令行rebootdnld功能，即对应菜单栏“chip-> Reboot to download”功能。

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6选1

Arguments:

    rebootdnld

For example:

    $ dtools.exe rebootdnld --debughost COM51,921600
      //此处发送数据为包头+FD 03 44 52

## shutdown
* shutdown device

>该命令是chip操作。该命令对应菜单栏“chip-> shutdown”功能。

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6选1

Arguments:

    shutdown

For example:

    $ dtools.exe shutdown --debughost COM51,921600
      //此处发送数据为包头+FD 03 46 4f

## profilesmode
* set profile mode

>该命令用来控制UE记录profile的方式，有0，1两种模式，0表示循环记录，1表示当记录满时停止profile。

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    //mode 6选1

Arguments:

    mode                         profile mode, 0/normal, 1/stop on full
For example:

    $ dtools.exe profilesmode 0 --debughost COM51,921600
      //此处发送数据为包头+FD 1d 00 00

# app image

## flashimgget   //todo
* get parameter from flash appimg

## flashimgset   //todo
* set parameter to flash appimg

## fileimgget   //todo
* get parameter from file appimg

## fileimgset   //todo
* set parameter to file appimg

# remote file system tools
* 此部分板子需与PC连接，且板子正常运行程序(live)状态
* remote指板子那一端，获取板子文件系统中的文件状态.此组命令对应dtools_win.exe中file manager中的相关功能。

Options:

    //连接方式从以下6选1
    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
## filesize
* remote file size and crc

Arguments:

    remote                       remote file name
For example:

    $ dtools.exe filesize modem/cp.bin.001 --debuguart COM51,921600
      modem/cp.bin.001 mode/0x81ff size/42032 crc/0x4cabfd4a

## getfile
* get file from remote

Arguments:

    remote                       remote file name
    local                        local file name
For example:

    $ dtools.exe getfile modem/cp.bin.000 ./outputdir/cp.bin.000 --debuguart COM51,921600
      read remote file, 8192/40592
      read remote file, 16384/40592
      read remote file, 24576/40592
      read remote file, 32768/40592
      read remote file, 40592/40592
## putfile
* put file to remote

Arguments:

    remote                       remote file name
    local                        local file name
For example:

    $ dtools.exe putfile /memdump.bin outputdir/memdump.bin --debuguart COM51,921600
      write remote file, 512/512

上传file后，可通过以下命令查看远程文件列表，检验是否成功

    $ dtools.exe listdir ./ --debuguart COM51,921600
         	./prepack/
        	./nvm/
         	./modemnvm/
       6292 ./simlock_data.bin
   	  37204 ./sms_dm_nv.bin
       	  8 ./cis_fw_context
         	./fota/
     	512 ./memdump.bin //此文件为putfile上传的文件

## rmfile
* delete remote file

Arguments:

    remote                       remote file name
For example:

    $ dtools.exe rmfile ./memdump.bin --debuguart COM51,921600
## rename
* rename remote file or directory

Arguments:

    remote-src                   remote source file or directory name
    remote-dst                   remote destination file or directory name

For example:

    $ dtools.exe rename ./memdump.bin ./mem.bin --debuguart COM51,921600  //修改文件名
    $ dtools.exe rename ./mem ./memdump --debuguart COM51,921600 //修改文件夹名

## listdir
* list remote directory

Arguments:

    remote                       remote directory
For example:

    $ dtools.exe listdir modem --debuguart COM51,921600
            12104 modem/bcpu_gsm.bin.000
            40592 modem/cp.bin.000
            42032 modem/cp.bin.001
            ...
            50056 modem/cp.bin.063
            18304 modem/cp.bin.064
              896 modem/mem_index_list
                  modem/nvm/
            17928 modem/riscV.bin.000
            43144 modem/zsp.bin.000
            42904 modem/zsp.bin.001
            ...
            10672 modem/zsp.bin.010
## listpart
* list remote partition

For example:

    $ dtools.exe listpart --debuguart COM51,921600
            / f_bsize/500 f_blocks/2751 f_bfree/2463 f_bavail/2463 f_flag/0
          /modem f_bsize/500 f_blocks/6527 f_bfree/189 f_bavail/189 f_flag/1
        /factory f_bsize/244 f_blocks/495 f_bfree/280 f_bavail/280 f_flag/1

## mkdir
* create remote directory

Arguments:

    remote                       directory in target
For example:

    $ dtools.exe mkdir ./test --debuguart COM51,921600

## rmdir
* delete remote directory

Arguments:

    remote                       directory in target
For example:

    $ dtools.exe rmdir ./test --debuguart COM51,921600

## mkpath
* create remote directory recursive

Arguments:

    remote                       directory in target
For example:

    $ dtools.exe mkpath ./test1/test2/3/4 --debuguart COM51,921600

## rmpath
* delete remote directory recursive

Arguments:

    remote                       directory in target
For example:

    $ dtools.exe rmpath ./test1 --debuguart COM51,921600
# elf tools

## elf2lod
* create lod from elf
* Usage: dtools.exe [options] elf2lod elf lod

>该命令用以将elf文件转换为lod文件，该功能只针对8909和8955，8910以后的芯片使用pac包。

Options:

    --start <start>      start symbol name, default to __flash_start
    --end <end>          end symbol name, default to __flash_end
    --param <param>      parameters to be added in lod header in: key=val, default to __flash_end
    --mips               convert address to mips physical address
    --boot               insert BOOT_SECTOR param

Arguments:

    elf                  elf file
    lod                  output lod file

For example:

    $ dtools.exe elf2lod ./pac/8910DM_cat1_v2.elf  ./pac/elf2lod.lod

## elf2bin
* create bin from elf
* Usage: dtools.exe [options] elf2bin elf bin

Options:

    --start <start>      start symbol name, default to __flash_start
    --end <end>          end symbol name, default to __flash_end
    --allow-empty        allow output empty file

Arguments:
  elf2bin
  elf                  elf file
  bin                  output bin file

For example:

    $ dtools.exe elf2bin ./pac/8910DM_cat1_v2.elf  ./pac/elf2lod.bin

## elf2romhex
* create romhex from elf

Options:

    --start <start>      start symbol name, default to __flash_start
    --end <end>          end symbol name, default to __flash_end
    --padding <padding>  pading 0/1, default 0
    --size <size>        list of rom block size, by comma

Arguments:

    elf                  input elf file
    hex                  output romhex file

For example:

    $ dtools.exe elf2romhex ./pac/8910DM_cat1_v2.elf  ./pac/elf2lodhex

## sectdump
* dump elf sections

Arguments:

    elf                  elf file name
    out                  output directory

For example:

    $ dtools.exe sectdump ./pac/8910DM_cat1_v2.elf  ./pac/elfsectdump

## mkuimage
* create uimage file from elf

Options:

    --start <start>      start symbol name, default to __flash_start
    --end <end>          end symbol name, default to __flash_end
    --name <name>        image name, default UIMAGE
    --timestamp <time>   timestamp in epoch seconds, default to now

Arguments:

    elf                  elf file
    img                  output image file

For example:

    $ dtools.exe mkuimage ./pac/8910DM_cat1_v2.elf  ./pac/elfuimage

## uimageinfo
* list uimage information

Arguments:

    img                  image file
For example:

    $ dtools.exe uimageinfo ./pac/elfuimage
      magic:  0x27051956
      hcrc:   0x8564310c OK
      time:   鍛ㄤ簩 鍥涙湀 27 16:32:50 2021 (1619512370)
      size:   2342496
      load:   0x60010040
      entry:  0x60010040
      dcrc:   0xc1b2befc OK
      name:   UIMAGE //此处name与mkuimage中--name option相对应

## mkappimg
* create appimg file from elf

Options:

    --start <start>      start symbol name, default to __flash_start
    --end <end>          end symbol name, default to __flash_end

Arguments:

    elf                  elf file
    img                  output image file
For example；

    $ dtools.exe mkappimg ./examples/appimg_hello_example_file.elf ./pac/elfappimg
    //会对elf文件进行判断，看是否为appimg elf文件，如不是会打印错误信息
    //判断标准 magic ？= 0x41505032； filedata size > 128
    //error 打印：elf is not an appimg

## appimginfo
* list appimg information

Arguments:

    img                  image file

For example:

    $ dtools.exe appimginfo ./pac/elfappimg
      magic:      0x41505032
      size:       544
      crc:        0x7883c444 OK
      stub:       1.0
      entry:      0x80f800e1
      exit:       0x80f8013d
      #0:         STUB 128/32/0x80f80080
      #1:         COPY 160/323/0x80f800a0
      #2:         COPY 512/0/0x80f80200
      #3:         COPY 512/4/0x80f80200
      #4:         BSS 0/4/0x80f80220
      #5:         (empty)

## mksimage
* create simage file from elf

Options:

    --bin                  binary input
    --load <load>          load addres of binary input
    --entry <entry>        entry addres of binary input
    --start <start>        start symbol name, default to __flash_start
    --end <end>            end symbol name, default to __flash_end
    --name <name>          image name
    --timestamp <time>     timestamp in epoch seconds, default to now
    --imgver <imgver>      image version in major.minor[.patch], default to 1.0.0
    --revision <revision>  image revision in decimal or hex, default to 0
    --platform <platform>  image platform
    --check <check>        check method used in image
* When input is raw binary file, --load and --entry must be specified.
And --start and --end will be ignored.
* When input is elf file,
--load and --entry will be ignored, --start and --end are optional.

Arguments:

    input                  input elf or bin file
    img                    output image file
For example:

    $ dtools.exe mksimage --platform list //通过该命令查看支持的platform
      supported platforms:
         8811
    $ dtools.exe mksimage ./pac/8910DM_cat1_v2.elf  ./pac/elfsimage

## mknullsimage
* create header only simage file

Options:

    --name <name>          image name
    --timestamp <time>     timestamp in epoch seconds, default to now
    --imgver <imgver>      image version in major.minor[.patch], default to 1.0.0
    --revision <revision>  image revision in decimal or hex, default to 0
    --platform <platform>  image platform
    --check <check>        check method used in image

Arguments:

    img                    output image file
For example:

    $ dtools.exe mknullsimage --check list
      supported check methods:
        crc, sha256
    $ dtools.exe mknullsimage --platform list
      supported platforms:
        8811
    $ dtools.exe mknullsimage --platform 8811 --check crc \
                 --name SIMAGE --imgver 1.0.0 ./pac/simgheader

## simageinfo
* list simage information

Arguments:

    img                  output image file
For example:

    $ dtools.exe simageinfo ./pac/elfsimage
      SIHR v1
    image size:         2342784
    image platform:     unknown
    image checksum:     crc
    image header size:  256
    image name:         SIMAGE
    image timestamp:    鍛ㄤ簩 鍥涙湀 27 17:23:25 2021
    image version:      1.0.0
    image revision:     0 0x00000000
    data:               256/2342496
    data flags:         0x0
    data load address:  0x60010040
    data entry address: 0x60010040
    priv data:          0/0/0x0
    key cert:           0/0
    priv data cert:     0/0
    debug primary cert: 0/0
    debug devel cert:   0/0
    header check:       OK
    payload check:      OK

# simage sign tools
 > 8811/8850处理对象为simage，通过ssign进行签名

 > 8910处理对象为uimage

## skeygen
* create key pair for simage

>该命令用以生成密钥对，最终结果为公钥与私钥两个密钥。

Options:

    --type <type>        public key algorithm: ec rsa
    --bits <bits>        rsa number of bits
    --curve <curve>      ec curve

Arguments:

    private_key_file     file for private key, in pem format
    public_key_file      file for public key, in pem format

For example:

    $ dtools.exe skeygen --type list
      supported types:
        ec, rsa,sm2
    $ dtools.exe skeygen --bits list
      supported bits:
        2048, 3072, 4096
    $ dtools.exe skeygen --curve list
     supported ec curves:
       secp192k1, secp192r1, secp224k1, secp224r1, secp256k1, secp256r1, secp384r1, secp521r1
    $ dtools.exe skeygen --type ec --curve secp224k1 privatekey publishkey
    $ dtools.exe skeygen --type rsa --bits 2048  privatekey publishkey
    $ dtools.exe skeygen --type sm2 privatekey publishkey


## ssign
* sign an image, create key cert
* Usage: dtools.exe [options] ssign root_pri root_pub keycert_version input output
* This is to sign an image. The existed inputimage must be unsigned.
* Unless --force is specified, this will report error if the output
image exists.
* keycert version will be checked for anti-rollback. So be careful
with this parameter. It is permitted that the keycert version keeps
the same when the change is trivial or small. And it must be
increased when there are major changes, especially secutiry fixes.
* Unless --resign is specified, this will report error is the input
image is already signed. At resign, the existed certs will be cleared.

Options:

    -r, --resign             clear existed certs
    -f, --force              overwite existed signed image
    --pkcsv15                use PKCS#1 v1.5 for rsa
    --debug_pub <debug_pub>  debug cert public key, in pem format

Arguments:

    root_pri                 file for root private key, in pem format
    root_pub                 file for root public key, in pem format
    keycert_version          security version in key cert
    input                    input unsigned image
    output                   output signed image
For example:

    $ dtools.exe ssign ./outputdir/privatekey ./outputdir/publishkey \
                 1 ./hex/8811CD_1618_LV.img outputdir/ssign

## ssigndebug
* sign an image, create debug cert
* Usage: dtools.exe [options] ssigndebug debug_pri debug_pub devel_pub debug_mask input output
* The input image should be signed image with key cert.

Options:

    -r, --resign         clear existed debug cert and devel cert
    -f, --force          overwite existed signed image
    --pkcsv15            use PKCS#1 v1.5 for rsa

Arguments:

    debug_pri            file for debug private key, in pem format
    debug_pub            file for debug public key, in pem format
    devel_pub            file for develop public key, in pem format
    debug_mask           debug mask in debug cert
    input                input signed image
    output               output signed image
For example:

    $ dtools.exe ssigndebug outputdir/prirsakey outputdir/pubrsakey \
                 outputdir/publishkey 8 hex/8811CD_1618_LV.sign.img \
                 outputdir/ssigndebug

## ssigndevel
* sign an image, create devel cert
* sage: dtools.exe [options] ssigndevel devel_pri devel_pub devel_mask soc_id input output
* The input image should be signed image with debug cert.

Options:

    -r, --resign         clear existed devel cert
    -f, --force          overwite existed signed image
    --pkcsv15            use PKCS#1 v1.5 for rsa

Arguments:

    devel_pri            file for develop private key, in pem format
    devel_pub            file for develop public key, in pem format
    devel_mask           debug mask in devel cert
    soc_id               soc id in hexdecimal format
    input                input signed image
    output               output signed image
For example:

    $ dtools.exe ssigndevel outputdir/prirsakey outputdir/pubrsakey \
                 outputdir/publishkey 8 12a3141516171819 hex/8811CD_1618_LV.sign.img \
                 outputdir/ssigndevel

## ssignchain
* sign an image, create chain cert
* sage: dtools.exe [options] ssignchain chain_pri chain_pub  input output
* The input image should be signed image with keycert.

Options:

    -r, --resign         clear existed devel cert
    -f, --force          overwite existed signed image
    --pkcsv15            use PKCS#1 v1.5 for rsa
    --soc_id<sod_id>     soc id in hexdecimal format

Arguments:

    chain_pri            file for chain private key, in pem format
    chain_pub            file for chain public key, in pem format
    input                input signed image with keycert
    output               output signed image with chaincert

For example:

    $ dtools.exe  skeygen $KEYGEN_OPT priv.pem pub.pem   //创建密钥对
    $ dtools.exe  skeygen $KEYGEN_OPT priv_chain.pem pub_chain.pem  //创建密钥对
    $ dtools.exe  ssign -r -f priv.pem pub.pem 1 boot.img boot.sign.img  //keycert签名
    $ dtools.exe ssignchain --soc_id 12a3141516171819  priv_chain.pem pub_chain.pem  \
    boot.sign.img boot_chain.sign.img

## ssignall
* sign an image, create key, debug,devel and chain cert
* Usage: dtools.exe [options] ssignall root_pri root_pub debug_pri debug_pub devel_pri devel_pub  \
    chain_pri chain_pub keycert_version debug_mask devel_mask soc_id input output

* This will create all certs: key cert, debug cert ,devel cert and chain cert

Options:

    -r, --resign         clear existed devel cert
    -f, --force          overwite existed signed image
    --pkcsv15            use PKCS#1 v1.5 for rsa

Arguments:

    ssignall
    root_pri             file for root private key, in pem format
    root_pub             file for root public key, in pem format
    debug_pri            file for debug private key, in pem format
    debug_pub            file for debug public key, in pem format
    devel_pri            file for develop private key, in pem format
    devel_pub            file for develop public key, in pem format
    chain_pri            file for chain private key, in pem format
    chain_pub            file for chain public key, in pem format
    keycert_version      security version in key cert
    debug_mask           debug mask in debug cert
    devel_mask           debug mask in devel cert
    soc_id               soc id in hexdecimal format
    input                input unsigned image
    output               output signed image
For example:

    $ dtools.exe  skeygen --type rsa --bits 4096 priv_rsa.pem pub_rsa.pem
    $ dtools.exe  skeygen --type sm2 priv_sm2.pem tpub_sm2.pem
    $ dtools.exe  skeygen --type ec --curve secp256k1 priv_ecc.pem pub_ecc.pem
    $ dtools.exe  skeygen --type rsa --bits 4096 priv_chain.pem pub_chain.pem
    $ dtools.exe  ssignall -f priv_rsa.pem pub_rsa.pem priv_ecc.pem pub_ecc.pem priv_sm2.pem pub_sm2.pem \
    priv_chain.pem pub_chain.pem 1 0xff 0xff 12a3141516171819 boot.img boot_ssign.sig.img

## ssignall8811
* sign an image, create key, debug and devel cert
* Usage: dtools.exe [options] ssignall8811 root_pri root_pub debug_pri debug_pub devel_pri \
    devel_pub  keycert_version debug_mask devel_mask soc_id input output

* This will create all certs: key cert, debug cert and devel cert.

Options:

    -r, --resign         clear existed devel cert
    -f, --force          overwite existed signed image
    --pkcsv15            use PKCS#1 v1.5 for rsa

Arguments:

    ssignall8811
    root_pri             file for root private key, in pem format
    root_pub             file for root public key, in pem format
    debug_pri            file for debug private key, in pem format
    debug_pub            file for debug public key, in pem format
    devel_pri            file for develop private key, in pem format
    devel_pub            file for develop public key, in pem format
    keycert_version      security version in key cert
    debug_mask           debug mask in debug cert
    devel_mask           debug mask in devel cert
    soc_id               soc id in hexdecimal format
    input                input unsigned image
    output               output signed image

For example:

    $ dtools.exe  skeygen --type rsa --bits 4096 priv_rsa.pem pub_rsa.pem
    $ dtools.exe  skeygen --type ec --curve secp256k1 priv_ecc.pem pub_ecc.pem
    $ dtools.exe  ssignall8811 -f priv_rsa.pem pub_rsa.pem priv_ecc.pem pub_ecc.pem priv_rsa.pem pub_rsa.pem \
    1 0xff 0xff 12345681233467712233 boot.img boot_ssign.sig.img

## sclearsign
* clear cert or certs in simage
* Usage: dtools.exe [options] sclearsign input output

Options:

    --keycert            clear key cert, debug cert and devel cert will be cleared also
    --debugcert          clear debug cert, devel cert will be cleared also
    --develcert          clear devel cert

Arguments:

    sclearsign
    input                input signed image
    output               output unsigned image

For example:

    $ dtools.exe skeygen --type rsa --bits 4096 priv_rsa.pem pub_rsa.pem
    $ dtools.exe ssign -r -f priv_rsa.pem pub_rsa.pem 1 $INPUT boot.sign.img
    $ dtools.ex sclearsign --keycert boot.sign.img boot.img

## sm2sign
* 该命令用以进行sm2签名
* Usage: dtools.exe [options] sm2sign pri hash

Arguments:

    pri                  private key pem file
    hash                 hash hex string

For example:

    $ dtools.exe sm2sign private.pem hash
    $ dtools.exe sm2sign private.pem 0102030405060708091011121314511617181920212223242526272829303132
    sig:3045022100d920d36ef3d02526bb90800b530c2224e1cea70d6367c41c3eab1db25b0b16b302207ebd7fbc1455e33a0e1cfdeb364eed8c1ead7412bb62e8fea901cf42a7ad989700
    r: d920d36ef3d02526bb90800b530c2224e1cea70d6367c41c3eab1db25b0b16b3
    s: 7ebd7fbc1455e33a0e1cfdeb364eed8c1ead7412bb62e8fea901cf42a7ad9897
    //hash必须为32bytes，命令行会打印签名后的文件内容，并显示sig_r与sig_s

# simage encrypt/decrypt tools

## sencrypt
* encrypt an image
* Usage: dtools.exe [options] sencrypt key iv input output

Options:

    --type<type>   encrypt mode: sm4, aes

Arguments:

    key                  the key for encrypt,should be 32 Bytes
    iv                   initializaton vector for encrypt,should be 16 Bytes
    input                input unencrypted image
    output               output encrypted image

For example：

    $ dtools.ex sencrypt --type sm4 06a9214036b8a15b512e03d53412000606a9214036b8a15b512e03d534120006 \
    3dafba429d9eb430b422da802c9fac41 boot.img test/boot.en.img

    $ dtools.ex sencrypt --type aes 06a9214036b8a15b512e03d53412000606a9214036b8a15b512e03d534120006 \
    3dafba429d9eb430b422da802c9fac41 boot.img test/boot.en.img

## sdecrypt
* decrypt an image
* Usage: dtools.exe [options] sdecrypt key iv input output

Options:

    --type<type>   encrypt mode: sm4, aes

Arguments:

    key                  the key for encrypt,should be 32 Bytes
    iv                   initializaton vector for encrypt,should be 16 Bytes
    input                input encrypted image
    output               output decrypted image

For example：

    $ dtools.ex sdecrypt --type sm4 06a9214036b8a15b512e03d53412000606a9214036b8a15b512e03d534120006 \
    3dafba429d9eb430b422da802c9fac41 boot.en.img  boot.de.img

# 8910 sign tools

## keyexp8910
* export public key from key.db

Options:

    --keydb <keydb>      key.db file path
    --pn <product>       product name in key.db

Arguments:

    pub                  output public key pem file
For example:

    $ dtools.exe keyexp8910 --keydb key.db --pn mytest pub

## imgverify8910
* verify 8910 signed image

Options:

    --boot               bootloader image
    --keydb              use key.db under executable directory
                         key.db 必须放在当前目录下
    --pn <product>       product name in key.db
    --pub <pub>          public key pem file

Arguments:

    image                signed image file
For example:

    $ dtools.exe imgverify8910 --keydb --pn test fdl2.sign.img
    $ dtools.exe imgverify8910 --pub pubkey.pem fdl2.sign.img
    $ dtools.exe imgverify8910 --keydb --pn test --boot boot.sign.img

## pacverify8910
* verify 8910 signed images in pac

Options:

    --keydb              use key.db under executable directory
    --pn <product>       product name in key.db
                         key.db 必须放在当前目录下
    --pub <pub>          public key pem file
    --id <id>            signed image ID in pac

Arguments:

    pac                  pac with signed image file

For example:

    $ dtools.exe pacverify8910 --keydb --pn test --id AP --id APPIMG 8910DM.pac
    $ dtools.exe pacverify8910 --pub pubkey.pem --id AP --id APPIMG 8910DM.pac

## sign8910
* key.db 可通过同级目录下的rsakeygen.exe工具生成
* sign 8910 image

Options:

    --keydb <keydb>      key.db file path
    --pn <product>       product name in key.db
    --pw <password>      optional 8 characters password
    --boot               bootloader image
    --nocheck            skip check whether the image is signed
    --resign             remove existed signature if existed
    --force              write output signed image even exist

Arguments:

    img                  input image
    out                  output signed image
For example：

    $ dtools.exe sign8910 --keydb key.db --pn mytest --pw 12345678 input.img outsign.img
## keydbinfo891
* Show key.db information。
该命令用以查看8910 key.db的详细信息。

Usage:

    dtools.exe [options] keydbinfo8910 keydb
For example：

    dtools.exe keydbinfo8910 key.db


# sffs tools

## fbdevdump1
* dump raw flash bin (version 1) to logical bin
    * phys_start = 0x1f4
    * phys_size = 0x197f
    * eb_size = 0x4d
    * pb_size = 0xffff
    * failed to load flash block device image

Arguments:

    fbdevdump1
    input                input raw flash bin
    output               output logical block device bin
For example:

    $ dtools.exe fbdevdump1 ./pac/cat1_64X128_UIS8915DM_V2_SingleSim.img ./pac/fbdevdump1.bin

## fbdevdump2
* dump raw flash bin (version 2) to logical bin

Arguments:

    flash                flash geometry in: start,size,eb_size,pb_size
    input                input raw flash bin
    output               output logical block device bin
For example:

    $ dtools.exe fbdevdump2 0x4a0000,0x340000,0x10000,0x200 \
                 pac/cat1_64X128_UIS8915DM_BB_RF_SingleSim.img pac/fbdevdump2.bin
    $ dtools.exe fbdevdump2 0x7e0000,0x20000,0x1000,0x100 \
                 pac/cat1_64X128_UIS8915DM_V2_SingleSim.img pac/flash.bin

## sffsdump
* extract logical bin or flash binto directory

Options:

    --flash <geom>       flash geometry in: start,size,eb_size,pb_size
       该option的四个参数可以根据coolwatcher的启动log进行查找
        FBDEV: create phys_start/0x340000 phys_size/0x160000 eb_size/0x8000 pb_size/0x200 read_only/0
        FBDEV: create phys_start/0x4a0000 phys_size/0x340000 eb_size/0x10000 pb_size/0x200 read_only/0
    --blocksize <bs>     block size, only needed for logical bin
    --offset <offset>    partition block offset, default to 0

Arguments:

    input                input logical bin, or flash bin
    output               output directory
For example:

    $ dtools.exe sffsdump --blocksize 500 ./pac/fbdevdump2.bin ./pac/sffsblock
    $ dtools.exe sffsdump --flash 0x4a0000,0x340000,0x10000,0x200 \
                 ./pac/cat1_64X128_UIS8915DM_V2_SingleSim.img ./pac/sffsfump

## fbdevgen
* generate flash bin

> The input json format:
* type: FBD1 or FBD2
* offset: flash offset
* size: flash block device size
* erase_block: erase block size
* logic_block: logic block size
* partition (array)
  * offset: logic block offset
  * count: logic block count
  * lzma_block_size: (optional) blocked LZMA size
  * plain_file (array)
    * file: file name in file system
    * local_file: local file name in PC
  * lzma3_file (array)
    * file: file name in file system, not including suffix
    * local_file: local file name in PC

Arguments:

    json                 json configuration file
    output               output image file

For example:

    $ dtools.exe fbdevgen outputdir/fbdevgen.json outputdir/outputimg
# pac tools

## pacinfo
* show .pac information

Arguments:

    pac                  input pac file
For example:

    $ dtools.exe pacinfo ./pac/8910DM_cat1_v2-cat1_64X128_UIS8915DM_V2_SingleSim-debug.pac
      === pac header ===
        Version:      BP_R1.0.0
        ProdName:     UIX8910_MODEM
        ...
      === nv item #0 ===
        Name:         Calibration
        ID:           0xffffffff
        ...
      === nv item #4 ===
        Name:         BT_Config
        ID:           0x191
            ...
      === file #0 ===
        ID:           HOST_FDL
        ...
      === file #10 ===
        ID:
        IDAlias:
        Name:         UIX8910_MODEM.xml
        ...
        ContentSize:  0xf53
## pacdump
* extract .pac to directory

Arguments:

    pac                  input pac file
    output               output directory
For example:

    $ dtools.exe pacdump ./pac/8910DM_cat1_v2-cat1_64X128_UIS8915DM_V2_SingleSim-debug.pac \
                ./outputdir
    //该命令将pac文件dump为*.img/*.bin/*.cpio/*.xml等文件

## pacmerge
* merge image from .pac to another. The <ext_id> must be only one, and can't exist in base pac.

Usage: dools.exe [options] pacmerge base ext out
Options:

    --id <id>          There are several kinds of format.
                       1. <ext_id,base_id>: copy <ext_id> before <base_id> in
                       base pac.
                       2. <ext_id,base_id,new_id>: copy <ext_id> before
                       <base_id> in base pac, and rename to <new_id>.
                       3. <ext_id,base_id,new_id,before|after>: copy <ext_id>
                       before or after <base_id> in base pac, and rename to
                       <new_id>. When <new_id> is empty, <ext_id> name will be
                       kept.
    // id可以通过查看pacdump命令得到的*.xml文件查找，默认是将external复制到base之前
    // 如需复制到base之后，则按照3设置after即可

Arguments:

    base                 base pac file
    ext                  external pac file
    output               output pac file
For example:

    $ dtools.exe pacmerge --id APPIMG,FLASH \
              ./pac/pacmerge/BC76_OPEN_CPU_LIB_00_V1.16-0100A1230201.pac \
              ./pac/pacmerge/1111.pac ./pac/pacmerge/out1.pac
    $ dtools.exe pacmerge --id PACKAGE_FILE,FLASH \
              ./pac/pacmerge/out1.pac ./pac/pacmerge/1111.pac \
              ./pac/pacmerge/out2.pac

## pacmultmerge
* merge image from .pac to another. The <ext_id> can be multiple, and can't exist in base pac.

Usage: dools.exe [options] pacmultmerge base ext out

Options:

    --id <id>          There are two kinds of format.
                       1. <ext_id1,ext_id2,...;base_id>: copy <ext_id*> before
                       <base_id> in base pac.
                       2. <ext_id1,ext_id2,...;base_id;before|after>: copy <ext_id*>
                       before or after <base_id> in base pac.
    // id可以通过查看pacdump命令得到的*.xml文件查找，默认是将external复制到base之前
    // 如需复制到base之后，则按照3设置after即可

Arguments:

    base                 base pac file
    ext                  external pac file
    output               output pac file
For example:

    $ dtools.exe pacmultmerge --id HZK1,HZK2,HZK3,HZK4;AP
              ./examples/N58-R07-STD-OE_V20-00D.pac \
              ./examples/hello_flash.pac ./pac/pacmerge/out1.pac
    $ dtools.exe pacmultmerge --id HZK1,HZK2,HZK3,HZK4,HZK5;AP;after
              ./examples/N58-R07-STD-OE_V20-00D.pac \
              ./examples/hello_flash.pac ./pac/pacmerge/out2.pac
    $ dtools.exe pacmultmerge --id HZK1;AP
              ./examples/N58-R07-STD-OE_V20-00D.pac \
              ./examples/hello_flash.pac ./pac/pacmerge/out3.pac

## pacsubst
* substitute image from .pac to another

> 该命令用以将base pac文件中的ID image，替换为ext pac文件中的ID image，然后保存为一个新文件。

Options:

    --id <id>            substitute id

Arguments:

    base                 base pac file
    ext                  external pac file
    output               output pac file
For example:

    $ dtools.exe pacsubst --id BOOTLOADER \
            ./pac/8910DM_cat1_v2-cat1_64X128_UIS8915DM_V2_SingleSim-debug.pac
            ./pac/04251017.pac 04251340.pac

## pacstrip
* strip image from .pac

Options:

    --id <id>            id to be stripped
Arguments:

    pac                  input pac file
    output               output directory
For example:

    $ dtools.exe pacstrip --id NV \
            ./pac/8910DM_cat1_v2-cat1_64X128_UIS8915DM_V2_SingleSim-debug.pac ./pac/0425131.pac

## fotacreate2
* create fota pack, format version

Options:

    --oldversion <oldver>              old version name, information to be
                                       embedded into fota
    --newversion <newver>              new version name, information to be
                                       embedded into fota
    --force                            create diff for same content, for debug
                                       only
    --single-pac <pac,xml>             single pac, without reference
    --pac <old,new,xml>                pac diff
    --lod <old,new,block_size,bundle>  lod diff

Arguments:

    output                             output image file

For example:

    $ dtools.exe fotacreate2 --single-pac ./pac/0413.pac,./setting/fota8910.xml \
                 outputdir/fota111.pac
    $ dtools.exe fotacreate2 --pac \
                 ./pac/v2_ext_merge.pac,./pac/v2_ext_merge2.pac,./setting/fota8910.xml \
                 ./pacmerge_ext.pac --oldversion "V1.0" --newversion "V1.1"
    //option中xml选择setting文件夹中对应的文件

## fotasign88xx
* generate fota signature file for fota pack

Options:

    -f, --force              overwite existed signed image
    --pkcsv15                use PKCS#1 v1.5 for rsa

Arguments:

    root_pri             file for root private key, in pem format
    root_pub             file for root public key, in pem format
    input                input fota pack file need to be signed
    output               output signature file

For example:

    $ dtools.exe fotasign88xx prv.pem pub.pem fota.pack fota.sign


## fotainfo
* show fota pack information

Arguments:

    pack                 fota pack file name
For example:

    $ dtools.exe fotainfo ./outputdir/fata111.pac
# nv tools

## nvprjdump
* dump nv prj into various format

Options:

    --out-bin <outbin>   output nv bin file
    --out-nv <outnv>     output nv values to file
    --out-prj <outprj>   output nv prj file
    //output file 三选一

Arguments:

    input                input prj/nv/bin files
    //input file类型三选一
For example:

    $ dtools.exe nvprjdump ./pac/8811_FEM_MIPI/nvm/nvitem.prj --out-bin ./pac/nv.bin

## nvprjdiff
* compare two nv prj files

Options:

    -n, --diff-only      difference only
Arguments:

    old                  old nv prj file
    new                  new nv prj file

For example:

    $ dtools.exe nvprjdiff ./pac/8811_FEM_MIPI/nvm/nvitem.prj ./pac/nvm/nvitem.prj --diff-only
    //比较结果共生成old.nv和new.nv两个文件
    // --diff-only为可选项，选择该option后，则只会比较在两个工程中同时存在的item

## deltanvmig
* delta nv migration

When nv project is changed by versions, delta nv can't be applied to
different version directly. This can migrate delta nv by a name map.

The name mapping file is a csv file. The first column is the original
name, and the second column is the new name. When --reverse is
specified, the reverse mapping will be used.

Options:

    -r, --reverse        reverse migration

Arguments:

    config               name mapping csv configuration file
    input                input delta nv file
    output               output delta nv file
For example：

    $  dtools.exe [options] deltanvmig config input output

## nvlist
* list nv in nv.bin

Arguments:

    nvbin                input nv.bin

For example:

    $ dtools.exe nvlist ./pac/nvm/nvitem.bin
    0x191: 40
    0x1ba: 268
    0x1f0: 152
    0x1f1: 104
    0x1f2: 200
    0x5: 8
    ...
    ...
    0x801: 436
    0x813: 2400
    0x870: 512
    0x871: 512
    0x872: 72
    0x873: 72

## nvdelta
* apply delta_nv.bin to nv.bin

Arguments:

    nvbin                input nv.bin
    deltanv              input delta_nv.bin
    output               output nv.bin
For example:

    $dtools.exe nvdelta ./pac/la.bin ./pac/delta_nv.bin ./pac/out.bin
## nvextract
* extract nv data from nv.bin

Options:

    -a, --all            extract all nvid
    --id <id>            nvid to be extracted
    //两个optionn二选一

Arguments:

    nvbin                input nv.bin
For example:

    $ dtools.exe nvextract ./pac/la.bin -a

# core export

## expgen
* generate export and stub

* It will generate 2 files
* core_export.o    it should be linked into core
* core_stub.o      it should be linked into app
* When calling this, make sure the corresponding compiler is located in PATH
* It is incompatible with version v1.0-37 abd bfore.

Options:

    -p, --platform <platform>  platform, 8910/8811/8850
    -e, --export <export>      export to be linked into core
    -s, --stub <stub>          stub to be linked into app

Arguments:

    list                       export list file name
For example:

    $  dtools.exe [options] expgen list

# lzmar tools
> 不需要连接板子，用于处理外部文件

## lzmare2
* lzmar compress, one output file

Options:

    --block-size <blocksize>  block size //可选项，如不设置，默认值为65536
Arguments:

    input                     input raw file
    output                    output compressed file
For example：

    $ dtools.exe lzmare2 ./pac/cat1_64X128_UIS8915DM_V2_SingleSim_la_nvitem.bin \
                         ./pac/lzmare2 --block-size 8192
## lzmare3
* lzmar compress, multiple output files

Options:

    --block-size <blocksize>  block size //可选项，如不设置，默认值为65536

Arguments:

    filesize                  raw data size for each output file
    input                     input raw file
    output                    output compressed file
For example:

    $dtools.exe lzmare3 16384 ./pac/cat1_64X128_UIS8915DM_V2_SingleSim_la_nvitem.bin \
                              ./pac/lzmare3 --block-size 1024
    //filesize > blocksize, 且 filesize % blocksize = 0
## lzmard2
* lzmar decompress, one output file

Arguments:

    input                input compressed file
    output               output raw file
For example:

    $dtools.exe lzmard2 ./pac/lzmare3.000 ./pac/lzmard2.bin
## lzmard3
* lzmar decompress, multiple output files

Arguments:

    input                input compressed file \
        //输入的inputfile为压缩文件名，然后自动匹配同名称的所有后缀*.000\*.001\...\*.00n的文件
    output               output raw file
For example:

    $dtools.exe lzmard3 ./pac/lzmare3 ./pac/lamard3.bin

# local tools

## hwcgen
   * generate C header from hardware.

Arguments:

    xml                  hardware XML file
    outdir               output directory
For example:

    $ dtools.exe hwcgen ./setting/chips/8811/8811_hard.xml outputdir/hwcgen

## hwsvdgen
   * generate SVD from hardware xml

Arguments:

    xml                  hardware XML file
    svd                  output SVD file
For example:

    $ dtools.exe hwsvdgen ./setting/chips/8811/8811_hard.xml outputdir/hwsvdgen
## lcrc
   * local file crc

Arguments:

    local                local file name
For example:

    $ dtools.exe lcrc ./outputdir/hwsvdgen
     0x02b76585 ./outputdir/hwsvdgen
## bin2lod
   * convert .bin to .lod

Options:

    --base <base>        base address of binary file

Arguments:

    bin                  input binary file
    lod                  output lod file
For example:

     $dtools.exe bin2lod --base 256  ./pac/partinfo.bin partinfo.lod
     --base必须设置，且4 bytes对齐

## lod2bin
   * convert .lod to .bin

Arguments:

    lod                  input lod file
    bin                  output binary file
For example:

     $dtools.exe lod2bin partinfo.lod partinfo.bin


## lodmerge
  * merge multiple lod files

Options：

    --bootmagic-all      handle BOOT_SECTOR of all inputs
    --bootmagic-none     don't handle BOOT_SECTOR of all inputs
    --align <align>      block address alignment
    --padding <padding>  pading 0/1, default 0
    --uppercase          upper case hexdecimal

Arguments:

    output               output lod file
    inputs               input lod files
For example:

    $dtools.exe lodmerge ./pac/merge.lod \
               ./pac/cat1_64X128_UIS8915DM_V2_SingleSim_la_nvitem.lod \
               ./pac/partinfo.lod

## comdump
   * dump uart raw data

Arguments:

    port                 uart port name
    baud                 uart port baud rate
For example:

    $dtools.exe comdump COM69 921600
    [20210421-16:34:28] received 19392 (19392 byte/s)
    [20210421-16:34:29] received 19392 (0 byte/s)
    [20210421-16:34:30] received 19392 (0 byte/s)
    [20210421-16:34:31] received 19728 (336 byte/s)
    [20210421-16:34:32] received 20172 (444 byte/s)
    [20210421-16:34:33] received 22032 (1860 byte/s)
## bin2prf
   * convert raw profile to prf

Arguments:

    bin                  input raw profile data file
    prf                  output prf file name
For example:

    $dtools.exe bin2prf ./pac/partinfo.bin ./pac/partinfo.prf
## cmmgen
   * generate cmm script from dump bin files

Options:

    --profile <profile>  profile in XML file
Arguments:

    dir                  directory
For example:

    $ dtools.exe cmmgen --profile list   //查看支持的profile list
    supported profiles:
        8910DM, 8955, 8955SX, 8908A, 8908ASX, 8909B, 8909BSX, 8811, 8811RF
    $ dtools.exe cmmgen ..\\BlueScreen --profile 8811 //BlueScreen路径需为*.bin文件存放路径

## bscinfo
   * parse bscore file and generate cmm script
   * 该功能需拿到bscore file

## stacklist
   * list memory, and possible symbols

Options:

    --uart <port,baud>           uart port and baud
    --debughost <port,baud>      debughost port and baud
    --debuguart <port,baud>      debuguart port and baud
    --coolhost <host:port>       coolhost socket server, host:port
    --diagport <port,baud>       diag uart port and baud
    --channelserver <host:port>  channelserver socket server, host:port
    --cmm <cmm>                  cmm file for offline mode
    --mips                       mips offline mode

Arguments:

    address                      starting address
    size                         size to be shown
    elf                          one or more elf file names
For example:

    $dtools.exe stacklist 0x00800000 512 ./pac/8910DM_cat1_v2.elf --debuguart COM51,921600
    0x00800000 0x2e073ab3
    0x00800004 0x2280280a
    0x00800008 0xcb5465ea
    0x0080000c 0x84b9f4ea
    ....
    0x0080018c 0x68639301
    0x00800190 0x6018e7ee <ipc_alloc_ul_ps_buf+0x22>
    ....
    0x008001b0 0x3408ffad
    0x008001b4 0x6018e7f3 <ipc_free_dl_ps_buf>
    0x008001b8 0x0000e7fb
    ....


## memlistinfo
   * parse and show memlist information

Arguments:

    memlist              memlist file

For example:

    $ dtools.exe memlistinfo ./pac/partinfo.bin
           PRTI~s` address/0x314c4653 size/0x53595346 flags/0x340000
                     address/0x314c4653 size/0x444f4d46 flags/0x4a0000
                     address/0x314c4653 size/0x43414646 flags/0x7e0000
                     address/0x53595346 size/0x2f       flags/0x0
                     address/0x00000000 size/0x0        flags/0x0
                     address/0x00000000 size/0x80       flags/0x0
      SFFSFMOD/modem address/0x00000000 size/0x0        flags/0x0
                     address/0x00000000 size/0x0        flags/0x0
                     address/0x43414646 size/0x6361662f flags/0x79726f74
                     address/0x00000000 size/0x0        flags/0x0
                     address/0x00000000 size/0x0        flags/0x7
      flag [0]:cpneed, [1]:ap read-only, [8]:lzma, [10]:lzma3

## gdbserver
   * dtools.exe [options] gdbserver elf
   * This is the gdb server. Optionally, it can launch gdb.
Options:
    --uart <port,baud>             uart port and baud
    --debughost <port,baud>        debughost port and baud
    --debuguart <port,baud>        debuguart port and baud
    --coolhost <host:port>         coolhost socket server, host:port
    --diagport <port,baud>         diag uart port and baud
    --channelserver <host:port>    channelserver socket server, host:port
    --cmm <cmm>                    cmm file for offline mode
    --mips                         mips offline mode
    --bscore <bscore>              bscore.bin for offline mode
    --nothread                     not parsing threads
    --listen <listen>              listen TCP socket port
    --mode <mode>                  GDB server mode
    --launch <gdb/insight>         launch gdb executable
    --gdbcmd <cmd>                 gdb command
    --symbol <sym=value>           additional symbols

Arguments:
    elf                          elf file name

For example:
    $ dtools.exe gdbserver --mode list
supported modes:
    8910AP, 8910CP, 8910APLive, 8910CPLive, 8850AP, 8850CP, 8850APLive, 8850CPLive, 8811,8811Live

$ dtools.exe gdbserver --mode 8910AP --cmm loadbin.cmm --launch setting/gdb/arm-gdb/bin/arm-none-eabi-gdb.exe  ap.elf

$ dtools.exe gdbserver --mode 8910AP --bscore bscore.bin  --launch setting/gdb/arm-gdb/bin/arm-none-eabi-gdb.exe ap.elf
