# μT-Kernel 3.0 BSP2 ユーザーズマニュアル  <!-- omit in toc -->
## ModusToolBox編  <!-- omit in toc -->
## Version 01.00.B0 <!-- omit in toc -->
## 2024.5.24  <!-- omit in toc -->

- [1. はじめに](#1-はじめに)
  - [1.1. 対象マイコンボード](#11-対象マイコンボード)
  - [1.2. 開発環境](#12-開発環境)
  - [1.3. ソフトウェアの構成](#13-ソフトウェアの構成)
- [2. BSP固有の機能について](#2-bsp固有の機能について)
  - [2.1. デバッグ用シリアル出力](#21-デバッグ用シリアル出力)
  - [2.2. 標準ヘッダファイルの使用](#22-標準ヘッダファイルの使用)
  - [2.3. FPU対応](#23-fpu対応)
- [3. デバイスドライバ](#3-デバイスドライバ)
  - [3.1. サンプル・デバイスドライバ(A/DC)](#31-サンプルデバイスドライバadc)
    - [3.1.1. 概要](#311-概要)
    - [3.1.2. デバイスドライバの使用方法](#312-デバイスドライバの使用方法)
  - [3.2. サンプル・デバイスドライバ(I2C)](#32-サンプルデバイスドライバi2c)
    - [3.2.1. 概要](#321-概要)
    - [3.2.2. デバイスドライバの使用方法](#322-デバイスドライバの使用方法)
- [4. プログラムの作成手順](#4-プログラムの作成手順)
  - [4.1. ModusToolboxによるプロジェクト作成](#41-modustoolboxによるプロジェクト作成)
  - [4.2. μT-Kernel 3.0 BSP2の組込み](#42-μt-kernel-30-bsp2の組込み)
    - [4.2.1. ソースコードの組込み](#421-ソースコードの組込み)
    - [4.2.2. ビルド設定の追加](#422-ビルド設定の追加)
  - [4.3. ユーザプログラム](#43-ユーザプログラム)
    - [4.3.1. ユーザプログラムの作成](#431-ユーザプログラムの作成)
    - [4.3.2. usermain関数](#432-usermain関数)
    - [4.3.3. プログラム例](#433-プログラム例)
  - [4.4. ビルドおよび実行](#44-ビルドおよび実行)
    - [4.4.1. プログラムのビルド](#441-プログラムのビルド)
    - [4.4.2. デバッグの設定](#442-デバッグの設定)
    - [4.4.3. プログラムの実行](#443-プログラムの実行)
- [5. 変更履歴](#5-変更履歴)

# 1. はじめに
本書はμT-Kernel 3.0 BSP2の使用法について説明します。  
μT-Kenrel 3.0 BSP2は、マイコン・メーカなどから提供されるマイコンの開発環境、ツールを使用し、また同様に提供されるファームウェアなどを活用してリアルタイムOS μT-Kernel 3.0を使用するためのBSP(Board Support Package)です。  

本書ではInfineon Technologies AGのマイコンXMCを搭載したマイコンボードのμT-Kernel 3.0 BSP2について説明します。

## 1.1. 対象マイコンボード
μT-Kenrel 3.0 BSP2は以下のXMCのマイコンボードに対応しています。

| マイコンボード       | マイコン        | CPUコア         | 備考         |
| ------------- | ----------- | ------------- | ---------- |
| KIT_XMC72_EVK | XMC7200D| ARM Cortex M7 Cortex M0+ | EVK-XMC7200 |

## 1.2. 開発環境
開発環境は、Infineon Technologies AGの統合開発環境ModusToolboxを使用します。  
また、ファームウェアとして、ModusToolboxの各種ソフトウェアを使用します。  
本書では以下のバージョンで動作を確認しています。  

`Eclipse IDE for ModusToolbox™ Version:  3.2.0`  

詳しくは以下のWebサイトをご覧ください。

ModusToolbox™ソフトウェア  
https://www.infineon.com/cms/jp/design-support/tools/sdk/modustoolbox-software/ 

## 1.3. ソフトウェアの構成
μT-Kernel 3.0 BSP2は、リアルタイムOS μT-Kernel 3.0と、対象マイコンボード用の依存部プログラムおよびサンプルのデバイスドライバから構成されます。  
μT-Kernel 3.0は以下のバージョンを使用しています。  

- μT-Kernel 3.0 (v3.00.07.B0)

μT-Kernel 3.0 BSP2のファイル構成を以下に示します。

- mtk3_bsp2  ルートディレクトリ  
  - config  コンフィギュレーション定義ファイル
  - doc   ドキュメント
  - include インクルードファイル
  - mtkernel  OS本体のソースコード
  - sysdepend マイコン依存部のソースコード
    - xmc_mtb  XMCマイコンおよびModuxToolbox用のソースコード
      - CPU CPU(ARMコア)依存部のソースコード
      - etc リンカスクリプト
      - lib ライブラリのハードウェア依存部
      - device　サンプル・デバイスドライバ
      

mtkernelディレクトリは、トロンフォーラムから公開されているμT-Kernel 3.0のGitサブモジュールです。なお、mtkernelディレクトリ下のソースコードは、ハードウェアに依存しない共通部のコードのみを使用しています。  
マイコンボードやファームウェアに依存するソースコードはsysdependディレクトリにあります。  
deviceディレクトリにはModusToolboxを利用したデバイスドライバのサンプル・プログラムがあります。I2CおよびA/DCの基本的な機能が使用可能です。  

# 2. BSP固有の機能について
## 2.1. デバッグ用シリアル出力
デバッグ用シリアル出力が使用できます。
この機能はコンフィギュレーションファイル(config/config.h)の以下を変更しビルドすることにより、有効無効を切り替えられます。

```c
/*---------------------------------------------------------------------- */
/* Use T-Monitor Compatible API Library  & Message to terminal.
 *  1: Valid  0: Invalid
 */
#define	USE_TMONITOR		(1)	/* T-Monitor API */
```

デバッグ用シリアル出力はT-Monitor互換API tm_printfが使用できます。tm_printfはC言語の標準関数printfとほぼ同等の機能を持ちますが、浮動小数点は使用できません。  
シリアル出力は以下の設定となります。  

| 項目       | 値         |
| -------- | --------- |
| 速度       | 115200bps |
| データ長     | 8bit      |
| パリティ     | なし        |
| Stop Bit | 1bit      |
| フロー制御    | 無し        |

シリアル信号はボードのデバッガKitProg3コネクタに接続されます。具体的には以下のマイコンのペリフェラルを使用しています。  

| マイコンボード       | ペリフェラル|
| ------------- | ----------- |
| EVK-XMC7200 | SCB3 |

## 2.2. 標準ヘッダファイルの使用
C言語の標準ヘッダファイルの使用が可能です。また、μT-Kernel 3.0のプログラムでも、<stddef.h>および<stdint.h>を使用しています。  
コンフィギュレーションファイル(config/config.h)の以下を変更することにより、標準ヘッダファイルは使用・不使用が指定できます。不使用とした場合、標準ヘッダファイルをユーザのプログラムからインクルードした際にエラーが発生することがあります。

```C
/*---------------------------------------------------------------------- */
/*
 *	Use Standard C include file
 */
#define USE_STDINC_STDDEF	(1)	/* Use <stddef.h> */
#define USE_STDINC_STDINT	(1)	/* Use <stdint.h> */
```

## 2.3. FPU対応
本BSPはマイコン内蔵のFPUに対応しています。タスクのコンテキスト情報にFPUのレジスタなどの情報を含めます。  
コンフィギュレーションファイル(config/config.h)の以下を変更することにより、FPUの使用・未使用が指定できます。ただし、FSPはFPUを有効にしますので、本BSPのFPU対応は常に有効にする必要があります。  

```C
/*---------------------------------------------------------------------- */
/* Use Co-Processor.
 *  1: Valid  0: Invalid
 */
#define	USE_FPU			(1)	/* Use FPU */
```

FPUを使用するタスクは、タスク属性にTA_FPUを指定する必要があります。ただし、本マイコンの開発環境では、プログラムの一部に対してFPUの使用・不使用を指定することはできません。よってFPUを使用する場合は、すべてのタスクの属性にTA_FPUを指定することを推奨します。  
なお、コンフィギュレーションファイル(config/config.h)の以下を1に設定することにより、タスク属性の値いに関係なしにすべてのタスクをFPU対応とすることができます。  

```C
#define	ALWAYS_FPU_ATR		(1)	/* Always set the TA_FPU attribute on all tasks */
```

# 3. デバイスドライバ
## 3.1. サンプル・デバイスドライバ(A/DC)
### 3.1.1. 概要
A/DCデバイスドライバは、マイコン内蔵のA/Dコンバータを制御することができます。  
本BSPでは以下のデバイスに対応したA/DCデバイスドライバがあります。  

| デバイス名 | BSPのデバイス名 | 説明                           |
| ----- | --------- | ---------------------------- |
| ADC   | sar2      | Analog-to-digital converters |

デバイスドライバは内部の処理でMTB CAT1 Peripheral driver libraryを利用します。本デバイスドライバはMTB CAT1 Peripheral driver libraryをμT-Kernel 3.0 で使用する方法を示すサンプルプログラムであり、デバイスの基本的な機能のみに対応しています。  
以下にA/DCデバイスドライバのソースコードがあります。  
 
```mtk3_bsp2/sysdepend/xmc_mtb/device/hal_adc```  

このデバイスドライバはBSPコンフィギュレーションファイル (config/config_bsp/xmc_mtb/config_bsp.h) の以下を変更しビルドすることにより、使用・不使用を切り替えられます。

```C
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_ADC		1	// A/D conversion device
```

このデバイスドライバは内部でMTB CAT1 Peripheral driver libraryを呼び出しますので、使用する際にはまず対象デバイスのModusToolboxの設定を行ってください。ModusToolboxとデバイスドライバの関連付けは次項で説明します。  

### 3.1.2. デバイスドライバの使用方法
(1) ModusToolboxの設定
A/DCデバイスドライバから使用するA/Dコンバータの設定をModusToolboxで行います。  
ModusToolboxでプロジェクトを右クリックして、メニューの中から`Device Configurator 4.20`を選び、`Pins`で使用するA/Dコンバータの端子を設定します。    

(参考) 各ボードのアナログ入力(A0～A15)と、マイコンのA/Dコンバータの入力の対応は以下の通りです。  

| アナログ入力 | EVK-XMC7200 | 備考 |
| ------------ | ----------- | ------------------ |
| A0           | SAR_ADC1_0  | Arduino Uno R3互換 |
| A1           | SAR_ADC1_1  | Arduino Uno R3互換 |
| A2           | SAR_ADC1_2  | Arduino Uno R3互換 |
| A3           | SAR_ADC1_3  | Arduino Uno R3互換 |
| A4           | SAR_ADC1_4  | Arduino Uno R3互換 |
| A5           | SAR_ADC1_5  | Arduino Uno R3互換 |
| A6           | SAR_ADC1_6  |    |
| A7           | SAR_ADC1_7  |    |
| A8           | SAR_ADC1_8  |    |
| A9           | SAR_ADC1_9  |    |
| A10          | SAR_ADC1_10 |    |
| A11          | SAR_ADC1_11 |    |
| A12          | SAR_ADC1_12 |    |
| A13          | SAR_ADC1_13 |    |
| A14          | SAR_ADC1_14 |    |
| A15          | SAR_ADC1_15 |    |


`Pins`の`Port 12`、`Port 14`から使用する入力を選択します。   
設定後に保存すると、ModusToolboxのビルドでソースコードが自動生成されます。  

(2) デバイスドライバの初期化  
A/DCデバイスドライバを使用するにあたり、最初に`dev_init_hal_adc`関数で初期化を行います。これにより、HALが関連付けられたA/DCデバイスドライバが生成されます。本関数は以下のように定義されます。  

```C
ER dev_init_hal_adc(
    UW unit        // デバイスのユニット番号
);
```

パラメータunitは0から順番に指定します。数字を飛ばすことはできません。   
初期化に成功するとデバイス名`hadc*`のデバイスドライバが生成されます。デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hadca`、ユニット番号1のデバイス名は`hadcb`となります。  

μT-Kernel 3.0 BSP2の起動処理の`knl_start_device`関数にてデバイスドライバの初期化を行っています。knl_start_device関数は以下のファイルに記述されています。  

`mtk3_bsp2/sysdepend/xmc_mtb/devinit.c`

以下にknl_start_device関数の内容を示します。ここではボードごとにArduino互換インタフェースのアナログ入力で使用されるデバイスドライバを初期化ししています。
初期化に成功するとデバイス名`hadc*`のデバイスドライバが生成されます。デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hadca`、ユニット番号1のデバイス名は`hadcb`、となります。  
実際に使用するA/Dコンバータに応じて変更してください。  

```C
EXPORT ER knl_start_device( void )
{
	ER	err	= E_OK;

        // 一部省略

#if DEVCNF_USE_HAL_ADC

	err = dev_init_hal_adc(DEV_HAL_ADC1);
	if(err < E_OK) return err;
#endif	/* DEVCNF_USE_HAL_ADC */

	return err;
}
```

(3) デバイスドライバの操作  
μT-Kernel 3.0のデバイス管理APIにより、デバイスドライバを操作できます。APIの詳細はμT-Kernel 3.0仕様書を参照してください。    
最初にオープンAPI tk_opn_devにて対象とするデバイス名を指定しデバイスをオープンします。  
オープン後は同期リードAPI tk_srea_devによりデータを取得することができます。パラメータのデータ開始位置にA/DCのチャンネルを指定します。  
本デバイスドライバでは、一度のアクセスで最大で17チャンネルからデータを取得できます。(チャンネル16はVBG(Bandgap voltage from SRSS)のデータです。)

以下にA/DCデバイスドライバを使用したサンプル・プログラムを示します。  
このプログラムは500ms間隔でA/DCのチャンネル0とチャンネル1からデータを取得し、その値をデバッグ用シリアル出力に送信するタスクの実行関数です。チャンネルの番号は実際に使用するA/Dコンバータに合わせてください。  

```C
LOCAL void task_1(INT stacd, void *exinf)
{
  UW	adc_val[2];
	ID	dd;     // デバイスディスクリプタ
	ER	err;    // エラーコード

	dd = tk_opn_dev((UB*)"hadcb", TD_UPDATE);    // デバイスのオープン
	while(1) {
		err = tk_srea_dev(dd, 0, &adc_val, 2, NULL);   // A/DC チャンネル0と1からデータを取得
		tm_printf((UB*)"A/DC A0 =%06d A/DC A1 =%06d\n", adc_val[0], adc_val[1]);    // デバッグ出力
		tk_dly_tsk(500);       // 500ms待ち
	}
}
```

## 3.2. サンプル・デバイスドライバ(I2C)
### 3.2.1. 概要
I2Cデバイスドライバは、マイコン内蔵のI2C通信デバイスを制御することができます。 
本BSPでは以下のデバイスに対応したI2Cデバイスドライバがあります。 

| デバイス名 | BSPのデバイス名 | 説明                                       |
| ------- | --------- | ---------------------------------------- |
| I2C     | hiic      | Inter-integrated circuit (I2C) interface |

デバイスドライバは内部の処理でMTB CAT1 Peripheral driver libraryを利用しています。このデバイスドライバはMTB CAT1 Peripheral driver libraryをμT-Kernel 3.0 で使用する方法を示すサンプルプログラムであり、デバイスの基本的な機能のみに対応しています。  
以下にI2Cデバイスドライバのソースコードがあります。

```mtk3_bsp2/sysdepend/xmc_mtb/device/hal_i2c```

このデバイスドライバはBSPコンフィギュレーションファイル (config/config_bsp/xmc_mtb/config_bsp.h) の以下を変更しビルドすることにより、使用・不使用を切り替えられます。

```C
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_IIC		1	// I2C communication device
```

I2CデバイスドライバはMTB CAT1 Peripheral driver libraryを使用していますので、MTB CAT1 Peripheral driver libraryのI2Cを使用可能にしてください。ModusToolboxと本デバイスドライバの関連付けは次項で説明します。  

### 3.2.2. デバイスドライバの使用方法
(1) ModusToolboxの設定
I2Cデバイスドライバから使用するSerial Communication Block(SCB)と端子の設定をModusToolboxで行います。  
ModusToolboxでプロジェクトを右クリックして、メニューの中から`Device Configurator 4.20`を選び、`Peripherals`で使用するSCBデバイス、`Pins`で使用する端子を設定します。    

(参考) 各ボードのArduino互換インタフェースのI2C信号と、マイコンのI2C端子の対応は以下の通りです。  

| ボードのI2C信号       | マイコンのI2C端子   |
| --------------- | ------------ |
| Arduino I2C SCL | P15.2/SCB9_SCL |
| Arduino I2C SDA | P15.1/SCB9_SDA |

`Device Configurator`で`Peripherals`から使用するSCBを選択し設定を行います。  
`Personality`にてSCBのモードを`I2C-4.0`に設定します。  
`Serial Communication Block(SCB)9 - Parameters`で`General`の`Mode`を`Master`に、`Connections`の`Clock`を`16 bit Divider 1 clk`に、`SCL`を`P15[2] digital_inout(CYBSP_I2C_SCL)`、 `SDA`を`P15[1] digital_inout(CYBSP_I2C_SDA)`に設定します。
`Pins`で`Port 15`の`P15[1]`と`P15[2]`を有効にし、それぞれの端子の`Parameters`で`General`の`Drive Mode`を`Open Drain, Drives High.Input buffer on`に、`Internal Connection`の`Digital InOut`を`Serial Communication Block(SCB)9 I2C.scl/sda`にします。その他の設定はデフォルトの設定値を前提としています。  
設定後に保存をすると、ModusToolboxのビルドでソースコードが自動生成されます。  

(2) デバイスドライバの初期化
I2Cデバイスドライバを使用するにあたり、最初に`dev_init_hal_i2c`関数で初期化を行います。これにより、指定したHALが関連付けられたI2Cデバイスドライバが生成されます。本関数は以下のように定義されます。   

```C
ER dev_init_hal_i2c(
    UW unit                // デバイスのユニット番号(0～DEV_HAL_I2C_UNITNM)
);
```
パラメータunitは0から順番に指定します。数字を飛ばすことはできません。   
初期化に成功するとデバイス名`hiic*`のデバイスドライバが生成されます。デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hiica`、ユニット番号1のデバイス名は`hiicb`、となります。

μT-Kernel 3.0 BSP2の起動処理のknl_start_device関数にてデバイスドライバの初期化を行っています。knl_start_device関数は以下のファイルに記述されています。  

`mtk3_bsp2/sysdepend/xmc_mtb/devinit.c`

以下にknl_start_device関数の内容を示します。ここは実際のデバイスドライバの使用に応じて変更してください。

```C
EXPORT ER knl_start_device( void )
{
    ER	err	= E_OK;

#if DEVCNF_USE_HAL_IIC

	  err = dev_init_hal_i2c(DEV_HAL_I2C9);
	  if(err < E_OK) return err;
#endif
        // 以下省略
}
```

(3) デバイスドライバの操作  
μT-Kernel 3.0のデバイス管理APIにより、デバイスドライバを操作できます。APIの詳細はμT-Kernel 3.0仕様書を参照してください。    
本デバイスドライバは、I2Cのコントローラ(マスター)モードにのみ対応します。  
最初にオープンAPI tk_opn_devにて対象とするデバイス名を指定しデバイスをオープンします。  
オープン後は同期リードAPI tk_srea_devによりデータの受信、同期ライトAPI tk_swri_devによりデータの送信を行うことができます。パラメータのデータ開始位置に対象のターゲット(スレーブ)アドレスを指定します。  

(4) デバイスのレジスタアクセス  
I2Cで接続されたターゲットデバイス内のレジスタをアクセスするための以下の関数が用意されています。これは比較的によく使用されるデバイス内のレジスタアクセス手順に対応しています。ただし、すべてのデバイスに使用できるわけではありませんので注意してください。

レジスタリード関数は、ターゲットデバイスに対して、レジスタアドレスの値(1byte)を送信し、続いてデータ(1byte)の受信を行います。  

```C
/* レジスタリード関数 */
ER i2c_read_reg(
    ID dd,    // デバイスディスクリプタ
    UW sadr,  // ターゲットアドレス
    UW radr,  // レジスタアドレス (下位8bitのみ有効)
    UB *data  // リードしたデータ(1byte)
);

```
レジスタライト関数は、ターゲットデバイスに対して、レジスタアドレスの値(1byte)を送信し、続いてデータ(1byte)の送信を行います。  

```C
/* レジスタライト関数 */
ER i2c_write_reg(
    ID dd,    // デバイスディスクリプタ
    UW sadr,  // ターゲットアドレス
    UW radr,  // レジスタアドレス (下位8bitのみ有効)
    UB data   // ライトするデータ(1byte)
);
```

# 4. プログラムの作成手順
ModusToolboxでプログラムのプロジェクトを作成し、μT-Kenrel 3.0 BSP2を組み込んでビルド、実行までの手順を説明します。

## 4.1. ModusToolboxによるプロジェクト作成
以下の手順で対象マイコンボードのプロジェクトを作成します。これはModusToolboxの標準の手順です。

(1) `Quick Panel`の[New Application]を選択します。  
(2) `Projector Creator`の画面の[Source Template]で`KIT_XMC72_EVK` 選んで、[Next]をクリックします。  
(3) `Template Application`で `Empty App`を選び、そこでプロジェクトの名前（例えば mtkernel3）を設定して、[Create]をクリックします。

マイコンのピン設定、コンフィギュレーションの基本的な設定はできていますで、開発するアプリケーションに応じて必要な設定を行ってください。

## 4.2. μT-Kernel 3.0 BSP2の組込み
### 4.2.1. ソースコードの組込み
作成したプロジェクトにμT-Kernel 3.0 BSP2のソースコードを組込みます。  
以下のGitHubのリポジトリからμT-Kernel 3.0 BSP2から入手し、プロジェクトのディレクトリ中にμT-Kernel 3.0 BSP2のソースコードのディレクトリmtk3_bsp2を置きます。

`https://github.com/tron-forum/mtk3_bsp2.git`  

gitのコマンドを使用する場合は、プロジェクトのディレクトリをカレントディレクトリとし、以下のコマンドを実行します。  

`git clone --recursive  https://github.com/tron-forum/mtk3_bsp2.git`  

μT-Kernel 3.0 BSP2はμT-Kernel 3.0のリポジトリをgitのサブモジュールとして内包していますので、全ソースコードを取得するには--recursiveが必要となります。

### 4.2.2. ビルド設定の追加
ModusToolboxのプロジェクトの設定ファイルに、μT-Kernel 3.0 BSP2のスースコードをビルドするための設定を追加します。  

(1) Makefileに対象マイコンボードのターゲット名の定義を追加します。

```
DEFINES=_XMCMTB_EVK_XMC7200_
```

対象マイコンボードとターゲット名の対応は以下の通りです。
|マイコンボード|ターゲット名|
|-|-|
| EVK-XMC7200 | \_XMCMTB_EVK_XMC7200_ |

### 4.2.3. OS起動処理の呼び出し
生成されたプロジェクトは、ハードウェアの初期化処理などのあと、main関数を実行します。  
main関数からμT-Kernel 3.0の起動処理knl_start_mtkernel関数を実行するようにコードを追加します。
main関数は以下のファイルの記述されています。

`<プロジェクトのディレクトリ>/main.c`

main関数に以下のようにknl_start_mtkernel関数を記述します。

```C
int main(void)
{
    cy_rslt_t result;

    /* 一部省略 */

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    void knl_start_mtkernel(void);
    knl_start_mtkernel();

    for (;;)
    {
    }
}
```

## 4.3. ユーザプログラム
### 4.3.1. ユーザプログラムの作成
μT-Kernel 3.0で実行するユーザプログラムを作成します。  
ユーザプログラムを作成するディレクトリは名前も場所も任意です。たとえば、プロジェクトのディレクトリ・ツリーの最上位にapplicationなどといった名称のディレクトリを作成します。

### 4.3.2. usermain関数
μT-Kernel 3.0は起動するとユーザプログラムのusermain関数を実行します。よってusermain関数を定義してください。  
usermain関数は以下の形式とします。

```C
INT usermain(void);
```

usermain関数は、μT-Kernel 3.0が最初に生成、実行したタスク（初期タスク）から呼ばれます。  
また、usermain関数が終了するとμT-Kernel 3.0はシャットダウンします。よって、通常はusermain関数は終了させずに、μT-Kernel 3.0のタスク休止API tk_slp_tskなどを使用して待ち状態とします。
初期タスクは優先度が高く設定されてますので、usermain関数の実行中は他のタスクは実行されません。

なお、ユーザプログラムでusermain関数を定義しなかった場合は、以下のファイルに記述されたデフォルトのusermain関数が実行されます。
このusermain関数は何もせずに終了します。よってμT-Kernel 3.0はすぐにシャットダウンをします。

`mtk3_bsp2/mtkernel/kernel/usermain/usermain.c`

デフォルトのusermain関数はweak属性が指定されてますので、ユーザプログラムにusermain関数が存在する場合は無効となります。ファイルを変更する必要はありません。

### 4.3.3. プログラム例
以下にユーザプログラムの例を示します。このプログラムは二つのタスクが実行されます。どちらのタスクも一定間隔でデバッグ用シリアル出力に文字列を表示します。

```C
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

LOCAL void task_1(INT stacd, void *exinf);	// task execution function
LOCAL ID	tskid_1;			// Task ID number
LOCAL T_CTSK ctsk_1 = {				// Task creation information
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= task_1,
	.tskatr		= TA_HLNG | TA_RNG3,
};

LOCAL void task_2(INT stacd, void *exinf);	// task execution function
LOCAL ID	tskid_2;			// Task ID number
LOCAL T_CTSK ctsk_2 = {				// Task creation information
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= task_2,
	.tskatr		= TA_HLNG | TA_RNG3,
};

LOCAL void task_1(INT stacd, void *exinf)
{
	while(1) {
		tm_printf((UB*)"task 1\n");
		tk_dly_tsk(500);
	}
}

LOCAL void task_2(INT stacd, void *exinf)
{
	while(1) {
		tm_printf((UB*)"task 2\n");
		tk_dly_tsk(700);
	}
}

/* usermain関数 */
EXPORT INT usermain(void)
{
	tm_putstring((UB*)"Start User-main program.\n");

	/* Create & Start Tasks */
	tskid_1 = tk_cre_tsk(&ctsk_1);
	tk_sta_tsk(tskid_1, 0);

	tskid_2 = tk_cre_tsk(&ctsk_2);
	tk_sta_tsk(tskid_2, 0);
	
	tk_slp_tsk(TMO_FEVR);

	return 0;
}
```

## 4.4. ビルドおよび実行
### 4.4.1. プログラムのビルド
プロジェクトを選択し、`Quick Panel`の`Build Application`または右クリックメニューにより、プロジェクトをビルドします。  
エラーが発生せずに正常に終了すれば実行プログラム(ELFファイル)が生成されています。  

### 4.4.2. デバッグの設定
使用するデバッガに応じてデバッグの設定を行います。
EVK-XMC7200ボードではデバッガはボード上に搭載されています。

### 4.4.3. プログラムの実行
マイコンボードをPCに接続し、`Quick Panel`の`<プロジェクト名> Debug (KitProg3_MiniProg4)`をクリックすると、実行プログラムがマイコンボードに転送され、デバッグ実行が開始します。`<プロジェクト名> Debug (KitProg3_MiniProg4)`が存在しない場合は`Generate Launches for <プロジェクト名>`をクリックすると、デバッグ用の構成が自動的に生成されます。
また、メニュー[Run]→[Debug]を選択すると、前回デバッグ実行した実行プログラムを再びデバッグ実行します。
実行プログラムはマイコンボードのフラッシュメモリーに書き込まれますので、デバッガを外した状態でマイコンボードの電源を入れれば、そのまま実行されます。


# 5. 変更履歴

| 版数      | 日付         | 内容   |
| ------- | ---------- | ---- |
| 1.00.B0 | 2024.05.24 | 新規作成 |
