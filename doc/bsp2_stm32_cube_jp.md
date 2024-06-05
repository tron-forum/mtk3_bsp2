# μT-Kernel 3.0 BSP2 ユーザーズマニュアル  <!-- omit in toc -->
## STM32Cube編  <!-- omit in toc -->
## Version 01.00.B3 <!-- omit in toc -->
## 2024.05.24  <!-- omit in toc -->

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
  - [4.1. STM32Cube IDEによるプロジェクト作成](#41-stm32cube-ideによるプロジェクト作成)
  - [4.2. μT-Kernel 3.0 BSP2の組込み](#42-μt-kernel-30-bsp2の組込み)
    - [4.2.1. ソースコードの組込み](#421-ソースコードの組込み)
    - [4.2.2. ビルド設定の追加](#422-ビルド設定の追加)
    - [4.2.3. OS起動処理の呼び出し](#423-os起動処理の呼び出し)
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

本書ではSTマイクロエレクトロニクス株式会社のマイコンSTM32を搭載したマイコンボードのμT-Kernel 3.0 BSP2について説明します。

## 1.1. 対象マイコンボード
μT-Kenrel 3.0 BSP2は以下のSTM32のマイコンボードに対応しています。

| マイコンボード       | マイコン        | CPUコア         | 備考         |
| ------------- | ----------- | ------------- | ---------- |
| NUCLEO-L476RG | STM32L476RG | Arm Cortex-M4 | STマイクロエレクトロニクス Nucleo-64  |
| NUCLEO-L4R5ZI | STM32L476RG | Arm Cortex-M4 | STマイクロエレクトロニクス Nucleo-64  |
| NUCLEO-F401RE | STM32F401RE | Arm Cortex-M4 | STマイクロエレクトロニクス Nucleo-64  |
| NUCLEO-F411RE | STM32L411RE | Arm Cortex-M4 | STマイクロエレクトロニクス Nucleo-64  |
| NUCLEO-F446RE | STM32F446RE | Arm Cortex-M4 | STマイクロエレクトロニクス Nucleo-64  |
| NUCLEO-G431KB | STM32G431KB | Arm Cortex-M4 | STマイクロエレクトロニクス Nucleo-64  |
| NUCLEO-G491RE | STM32G491RE | Arm Cortex-M4 | STマイクロエレクトロニクス Nucleo-64  |
| NUCLEO-F767ZI | STM32F767ZI | Arm Cortex-M7 | STマイクロエレクトロニクス Nucleo-144 |
| NUCLEO-H723ZG | STM32H723ZG | Arm Cortex-M7 | STマイクロエレクトロニクス Nucleo-144 |

## 1.2. 開発環境
開発環境は、STマイクロエレクトロニクス株式会社の統合開発環境STM32Cube IDEを使用します。  
また、ファームウェアとして、STM32Cubeの各種ソフトウェアを使用します。  
本書では以下のバージョンで動作を確認しています。  

`STM32CubeIDE Version: 1.13.2`  

詳しくは以下のWebサイトをご覧ください。

STM32用統合開発環境  
https://www.st.com/ja/development-tools/stm32cubeide.html  

## 1.3. ソフトウェアの構成
μT-Kernel 3.0 BSP2は、リアルタイムOS μT-Kernel 3.0と、対象マイコンボード用の依存部プログラムおよびサンプルのデバイスドライバから構成されます。  
μT-Kernel 3.0は以下のバージョンを使用しています。  

- μT-Kernel 3.0 (v3.00.07.B0)

μT-Kernel 3.0 BSP2のファイル構成を以下に示します。

- mtk3_bsp2  ルートディレクトリ  
  - config  コンフィギュレーション定義ファイル
  - include インクルードファイル
  - mtkernel  OS本体のソースコード
  - sysdepend マイコン依存部のソースコード
    - stm32_cube  STM32マイコンおよびSTM32Cube用のソースコード
      - CPU CPU(ARMコア)依存部のソースコード
      - lib ライブラリのハードウェア依存部
      - device　サンプル・デバイスドライバ
      - doc   ドキュメント

mtkernelディレクトリは、トロンフォーラムから公開されているμT-Kernel 3.0のGitサブモジュールです。なお、mtkernelディレクトリ下のソースコードは、ハードウェアに依存しない共通部のコードのみを使用しています。  
マイコンボードやファームウェアに依存するソースコードはsysdependディレクトリにあります。  
deviceディレクトリにはSTM32Cubeを利用したデバイスドライバのサンプル・プログラムがあります。I2CおよびA/DCの基本的な機能が使用可能です。  

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

シリアル信号はボードのデバッガST-LINK micro USBコネクタに接続されます。具体的には以下のマイコンのペリフェラルを使用しています。  

| マイコンボード       | ペリフェラル|
| ------------- | ----------- |
| NUCLEO-L476RG | USART2 |
| NUCLEO-L4R5ZI | LPUART1|
| NUCLEO-F401RE | USART2 |
| NUCLEO-F411RE | USART2 |
| NUCLEO-F446RE | USART2 |
| NUCLEO-G431KB | LPUART1|
| NUCLEO-G491RE | LPUART1|
| NUCLEO-F767ZI | USART3 |
| NUCLEO-H723ZG | USART3 |

## 2.2. 標準ヘッダファイルの使用
C言語の標準ヘッダファイルの使用が可能です。また、μT-Kernel 3.0のプログラムでも、<stddef.h>および<stdint.h>を使用しています。  
コンフィギュレーションファイル(config/config.h)の以下を変更することにより、標準ヘッダファイルは使用・不使用が指定できます。不使用とした場合、標準ヘッダファイルをユーザのプログラムからインクルードした際にエラーが発生することがあります。

```C
/*---------------------------------------------------------------------- */
/*
 *	Use Standard C include file
 */
#define USE_STDINC_STDDEF	(1)	/* Use <stddef.h> */
#define USE_STDINC_STDINT	(1) /* Use <stdint.h> */
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
| ADC   | hadc      | Analog-to-digital converters |

デバイスドライバは内部の処理でSTM32CubeFWを利用します。本デバイスドライバはFSTM32CubeFWをμT-Kernel 3.0 で使用する方法を示すサンプルプログラムであり、デバイスの基本的な機能のみに対応しています。  
以下にA/DCデバイスドライバのソースコードがあります。  
 
```mtk3_bsp2/sysdepend/stm32_cube/device/hal_adc```  

このデバイスドライバはBSPコンフィギュレーションファイル (config/config_bsp/stm32_cube/config_bsp.h) の以下を変更しビルドすることにより、使用・不使用を切り替えられます。

```C
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_ADC		1	// A/D conversion device
```

このデバイスドライバは内部でSTM32CubeFWを呼び出しますので、使用する際にはまず対象デバイスのSTM32CubeFWの設定を行ってください。STM32CubeFWとデバイスドライバの関連付けは次項で説明します。  

### 3.1.2. デバイスドライバの使用方法
(1) STM32CubeFWの設定
A/DCデバイスドライバから使用するA/Dコンバータの設定をSTM32CubeIDEで行います。  
プロジェクトの中の拡張子iocのファイルが設定ファイルです。STM32CubeIDEでこのファイルを開きます。  
`Pinout & Configuration`の`Pinout`で使用するA/Dコンバータの端子を設定します。  

(参考) 各ボードのArduino互換インタフェースのアナログ入力(A0～A5)と、マイコンのA/Dコンバータの入力の対応は以下の通りです。  

| Arduinoアナログ入力 | F401,F411 | F446        | L476       |
| ------------- | --------- | ----------- | ---------- |
| A0            | ADC1_0    | ADC123_IN0  | ADC12_IN5  |
| A1            | ADC1_1    | ADC123_IN1  | ADC12_IN6  |
| A2            | ADC1_4    | ADC12_IN4   | ADC12_IN9  |
| A3            | ADC1_8    | ADC12_IN8   | ADC12_IN15 |
| A4            | ADC1_11   | ADC123_IN11 | ADC123_IN2 |
| A5            | ADC1_10   | ADC123_IN10 | ADC123_IN1 |


| Arduinoアナログ入力 | H723         | F767        | L4R5       | G431, G491             |
| ------------- | ------------ | ----------- | ---------- | ---------------------- |
| A0            | ADC12_INP15  | ADC123_IN3  | ADC12_IN8  | ADC12_IN1              |
| A1            | ADC123_INP10 | ADC123_IN10 | ADC123_IN1 | ADC12_IN2              |
| A2            | ADC12_INP13  | ADC123_IN13 | ADC123_IN4 | ADC2_IN17              |
| A3            | ADC12_INP5   | ADC3_IN9    | ADC123_IN2 | ADC3_IN12 or ADC1_IN15 |
| A4            | ADC123_INP12 | ADC3_IN15   | ADC12_IN13 | ADC12_IN7              |
| A5            | ADC3_INP6    | ADC3_IN8    | ADC12_IN14 | ADC12_IN6              |

`Pinout & Configuration`の`Software Packs`で`Analog`から使用するA/Dコンバータを選択し設定を行います。  
`Mode`にてA/Dコンバータの使用する入力を`Single-ended`に設定します。  
`Configuration`の`NVIC Settings`で割り込みを有効にします。その他の設定はデフォルトの設定値を前提としています。  
割り込み優先度は`System Core`から`NVIC`を選択して設定を行いします。  
設定後にiocファイルを保存すると、STM32CubeFWのソースコードが自動生成されます。  

(2) デバイスドライバの初期化  
A/DCデバイスドライバを使用するにあたり、最初に`dev_init_hal_adc`関数で初期化を行います。これにより、HALが関連付けられたA/DCデバイスドライバが生成されます。本関数は以下のように定義されます。  

```C
ER dev_init_hal_adc(
    UW unit,        // デバイスのユニット番号(0～DEV_HAL_ADC_UNITNM)
    ADC_HandleTypeDef *hadc         // ADCハンドラ構造体
);
```

パラメータunitは0から順番に指定します。数字を飛ばすことはできません。  
パラメータhadcは、STM32CubeIDEで自動的に生成される情報です。  
初期化に成功するとデバイス名`hadc*`のデバイスドライバが生成されます。デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hadca`、ユニット番号1のデバイス名は`hadcb`となります。  

μT-Kernel 3.0 BSP2の起動処理の`knl_start_device`関数にてデバイスドライバの初期化を行っています。knl_start_device関数は以下のファイルに記述されています。  

`mtk3_bsp2/sysdepend/stm32_cube/devinit.c`

以下にknl_start_device関数の内容を示します。ここではボードごとにArduino互換インタフェースのアナログ入力で使用されるデバイスドライバを初期化ししています。STM32CubeFWの`hadc1`,`hadc2`,`hadc3`が関連付けられたμT-Kernel 3.0のデバイスドライバが生成されます。  
実際に使用するA/Dコンバータに応じて変更してください。  

```C
EXPORT ER knl_start_device( void )
{
	ER	err	= E_OK;

        // 一部省略

#if DEVCNF_USE_HAL_ADC
	IMPORT ADC_HandleTypeDef	hadc1;
	err = dev_init_hal_adc(DEV_HAL_ADC1, &hadc1);
	if(err < E_OK) return err;

#if defined(MTKBSP_NUCLEO_STM32H723) || defined(MTKBSP_NUCLEO_STM32F767)
	IMPORT ADC_HandleTypeDef	hadc3;
	
	err = dev_init_hal_adc(DEV_HAL_ADC2, NULL);	// hadc2 is not used
	if(err < E_OK) return err;
	err = dev_init_hal_adc(DEV_HAL_ADC3, &hadc3);
	if(err < E_OK) return err;

#elif defined(MTKBSP_NUCLEO_STM32G431) || defined(MTKBSP_NUCLEO_STM32G491)
	IMPORT ADC_HandleTypeDef	hadc2;
	
	err = dev_init_hal_adc(DEV_HAL_ADC2, &hadc2);
	if(err < E_OK) return err;
	err = dev_init_hal_adc(DEV_HAL_ADC3, NULL);	// hadc3 is not used.
	if(err < E_OK) return err;
#else
//#elif defined(MTKBSP_NUCLEO_STM32L476) || defined(MTKBSP_NUCLEO_STM32F401) || defined(MTKBSP_NUCLEO_STM32F411) || defined(MTKBSP_NUCLEO_STM32F446)
	err = dev_init_hal_adc(DEV_HAL_ADC2, NULL);	// hadc2 is not used
	if(err < E_OK) return err;
	err = dev_init_hal_adc(DEV_HAL_ADC3, NULL);	// hadc3 is not used
	if(err < E_OK) return err;

#endif
#endif	/* DEVCNF_USE_HAL_ADC */

	return err;
}
```

(3) デバイスドライバの操作  
μT-Kernel 3.0のデバイス管理APIにより、デバイスドライバを操作できます。APIの詳細はμT-Kernel 3.0仕様書を参照してください。    
最初にオープンAPI tk_opn_devにて対象とするデバイス名を指定しデバイスをオープンします。  
オープン後は同期リードAPI tk_srea_devによりデータを取得することができます。パラメータのデータ開始位置にA/DCのチャンネルを指定します。  
本デバイスドライバでは、一度のアクセスで一つのチャンネルから1データのみを取得できます。  

以下にA/DCデバイスドライバを使用したサンプル・プログラムを示します。  
このプログラムは500ms間隔でA/DCのチャンネル9とチャンネル0からデータを取得し、その値をデバッグ用シリアル出力に送信するタスクの実行関数です。チャンネルの番号は実際に使用するA/Dコンバータに合わせてください。  

```C
LOCAL void task_1(INT stacd, void *exinf)
{
  UW	adc_val1, adc_val2;
	ID	dd;     // デバイスディスクリプタ
	ER	err;    // エラーコード

	dd = tk_opn_dev((UB*)"hadca", TD_UPDATE);    // デバイスのオープン
	while(1) {
		err = tk_srea_dev(dd, 9, &adc_val1, 1, NULL);   // A/DC チャンネル9からデータを取得
		err = tk_srea_dev(dd, 0, &adc_val2, 1, NULL);   // A/DC チャンネル0からデータを取得
		tm_printf((UB*)"A/DC A0 =%06d A/DC A1 =%06d\n", adc_val1, adc_val2);    // デバッグ出力
		tk_dly_tsk(1000);       // 500ms待ち
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

デバイスドライバは内部の処理でSTM32CubeFWを利用しています。このデバイスドライバはSTM32CubeFWをμT-Kernel 3.0 で使用する方法を示すサンプルプログラムであり、デバイスの基本的な機能のみに対応しています。  
以下にI2Cデバイスドライバのソースコードがあります。

```mtk3_bsp2/sysdepend/stm32_cube/device/hal_i2c```

このデバイスドライバはBSPコンフィギュレーションファイル (config/config_bsp/stm32_cube/config_bsp.h) の以下を変更しビルドすることにより、使用・不使用を切り替えられます。

```C
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_IIC		1	// I2C communication device
```

I2CデバイスドライバはSTM32CubeFWを使用していますので、STM32CubeFWのI2Cを使用可能にしてください。STM32CubeFWと本デバイスドライバの関連付けは次項で説明します。  

### 3.2.2. デバイスドライバの使用方法
(1) STM32CubeFWの設定
I2Cデバイスドライバから使用するA/Dコンバータの端子設定をSTM32CubeIDEで行います。  
プロジェクトの中の拡張子iocのファイルが設定ファイルです。STM32CubeIDEでこのファイルを開きます。  
`Pinout & Configuration`の`Pinout`で使用するI2Cコンバータの端子を設定します。  

(参考) 各ボードのArduino互換インタフェースのI2C信号と、マイコンのI2C端子の対応は以下の通りです。  

| ボードのI2C信号       | マイコンのI2C端子   |
| --------------- | ------------ |
| Arduino I2C SCL | PB8/I2C1_SCL |
| Arduino I2C SDA | PB9/I2C1_SDA |

`Pinout & Configuration`の`Software Packs`で`Connectivity`から使用するI2Cを選択し設定を行います。  
`Mode`にてI2Cのモードを`I2C`に設定します。  
`Configuration`の`NVIC Settings`で割り込みを有効にします。その他の設定はデフォルトの設定値を前提としています。  
割り込み優先度は`System Core`から`NVIC`を選択して設定を行いします。  
設定後にiocファイルを保存すると、STM32CubeFWのソースコードが自動生成されます。  

(2)) デバイスドライバの初期化
I2Cデバイスドライバを使用するにあたり、最初に`dev_init_hal_i2c`関数で初期化を行います。これにより、指定したHALが関連付けられたI2Cデバイスドライバが生成されます。本関数は以下のように定義されます。   

```C
ER dev_init_hal_i2c(
    UW unit,                // デバイスのユニット番号(0～DEV_HAL_ADC_UNITNM)
    I2C_HandleTypeDef *hi2c // I2Cハンドル構造体
);
```
パラメータunitは0から順番に指定します。数字を飛ばすことはできません。  
パラメータhai2cは、STM32Cubeを設定すると自動的に生成されるI2Cの情報です。 
初期化に成功するとデバイス名`hiic*`のデバイスドライバが生成されます。デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hiica`、ユニット番号1のデバイス名は`hiicb`、となります。

μT-Kernel 3.0 BSP2の起動処理のknl_start_device関数にてデバイスドライバの初期化を行っています。knl_start_device関数は以下のファイルに記述されています。  

`mtk3_bsp2/sysdepend/stm32_cube/devinit.c`

以下にknl_start_device関数の内容を示します。ここは実際のデバイスドライバの使用に応じて変更してください。

```C
EXPORT ER knl_start_device( void )
{
    ER	err	= E_OK;

#if DEVCNF_USE_HAL_IIC
	  IMPORT I2C_HandleTypeDef	hi2c1;

	  err = dev_init_hal_i2c(DEV_HAL_I2C1, &hi2c1);
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
    UB data  // ライトするデータ(1byte)
);
```

# 4. プログラムの作成手順
STM32Cube IDEでプログラムのプロジェクトを作成し、μT-Kenrel 3.0 BSP2を組み込んでビルド、実行までの手順を説明します。

## 4.1. STM32Cube IDEによるプロジェクト作成
以下の手順で対象マイコンボードのプロジェクトを作成します。これはSTM32Cube IDEの標準の手順です。

(1) メニュー[File]→[New]→[STM32 Project]を選択します。  
(2) 対象のマイコンボードを選択します。  
(3) 以下の設定のプロジェクトを作成します。  
- Target Language : C
- Target Binary Type : Executable
- Target Project Type : STM32Cube

マイコンのピン設定、コンフィギュレーションの基本的な設定はできていますで、開発するアプリケーションに応じて必要な設定を行ってください。

## 4.2. μT-Kernel 3.0 BSP2の組込み
### 4.2.1. ソースコードの組込み
作成したプロジェクトにμT-Kernel 3.0 BSP2のソースコードを組込みます。  
以下のGitHubのリポジトリからμT-Kernel 3.0 BSP2から入手し、プロジェクトのディレクトリ中にμT-Kernel 3.0 BSP2のソースコードのディレクトリmtk3_bsp2を置きます。

`https://github.com/tron-forum/mtk3_bsp2.git`  

gitのコマンドを使用する場合は、プロジェクトのディレクトリをカレントディレクトリとし、以下のコマンドを実行します。  

`git clone --recursive  https://github.com/tron-forum/mtk3_bsp2.git`  

μT-Kernel 3.0 BSP2はμT-Kernel 3.0のリポジトリをgitのサブモジュールとして内包していますので、全ソースコードを取得するには--recursiveが必要となります。  

取り込んだμT-Kernel 3.0 BSP2のディレクトリは、STM32Cube IDEのビルド対象外の設定になっている可能性がありますので、プロパティを変更します。  
`Exclude resource from build`にチェックが入っている場合は外してください。  

### 4.2.2. ビルド設定の追加
プロジェクトのプロパティに、μT-Kernel 3.0 BSP2のスースコードをビルドするための設定を追加します。  
[C/C++ Builed]→[Settings]→[Tool Settings]で以下を設定します。

(1) [MCU GCC Compiler]→[Preprocessor]  
[Define symbles]に対象マイコンボードのターゲット名を設定します。

|マイコンボード|ターゲット名|
|-|-|
| NUCLEO-L476RG | \_STM32CUBE_NUCLEO_L476_ |
| NUCLEO-L4R5ZI | \_STM32CUBE_NUCLEO_L4R5_ |
| NUCLEO-F401RE | \_STM32CUBE_NUCLEO_F401_ |
| NUCLEO-F411RE | \_STM32CUBE_NUCLEO_F411_ |
| NUCLEO-F446RE | \_STM32CUBE_NUCLEO_F446_ |
| NUCLEO-G431KB | \_STM32CUBE_NUCLEO_G431_ |
| NUCLEO-G491RE | \_STM32CUBE_NUCLEO_G491_ |
| NUCLEO-F767ZI | \_STM32CUBE_NUCLEO_F767_ |
| NUCLEO-H723ZG | \_STM32CUBE_NUCLEO_H723_ |

(2) [MCU GCC Compiler]→[include paths]  
[Include paths]に以下を設定します。  
プロジェクトのディレクトリの直下にμT-Kernel 3.0 BSP2のディレクトリmtk3_bsp2がある前提です。   

```
"${workspace_loc:/${ProjName}/mtk3_bsp2}"
"${workspace_loc:/${ProjName}/mtk3_bsp2/config}"
"${workspace_loc:/${ProjName}/mtk3_bsp2/include}"
"${workspace_loc:/${ProjName}/mtk3_bsp2/mtkernel/kernel/knlinc}"
```

(3) [MCU GCC Assembler]→[Preprocessor]  
(1)と同じ設定を行います。  

(4) [MCU GCC Assembler]→[include paths]  
(2)と同じ設定を行います。  

### 4.2.3. OS起動処理の呼び出し
生成されたプロジェクトは、ハードウェアの初期化処理などのあと、main関数を実行します。  
main関数からμT-Kernel 3.0の起動処理knl_start_mtkernel関数を実行するようにコードを追加します。
main関数は以下のファイルの記述されています。

`<プロジェクトのディレクトリ>/Core/Src/main.c`

main関数の`/* USER CODE BEGIN 2 */`と`/* USER CODE END 2 */`の間に以下のように記述します。

```C
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  
  void knl_start_mtkernel(void);
  knl_start_mtkernel();

  /* USER CODE END 2 */

  /** 以下省略 **/
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
プロジェクトを選択し、メニュー[Projects]→[Build project]または右クリックメニューにより、プロジェクトをビルドします。  
エラーが発生せずに正常に終了すれば実行プログラム(ELFファイル)が生成されています。  

### 4.4.2. デバッグの設定
使用するデバッガに応じてデバッグの設定を行います。
STM32 Nucleoボードではデバッガはボード上に搭載されています。

### 4.4.3. プログラムの実行
マイコンボードをPCに接続し、メニュー[RUn]→[Debug configurations]を選び、設定を確認したのち、[Debug]ボタンをクリックすると、実行プログラムがマイコンボードに転送され、デバッグ実行が開始します。
また、メニュー[Run]→[Debug]を選択すると、前回デバッグ実行した実行プログラムを再びデバッグ実行します。
実行プログラムはマイコンボードのフラッシュメモリーに書き込まれますので、デバッガを外した状態でマイコンボードの電源を入れれば、そのまま実行されます。


# 5. 変更履歴

| 版数      | 日付         | 内容   |
| ------- | ---------- | ---- |
| 1.00.B3 | 2024.05.24 | 誤記修正 |
| 1.00.B2 | 2024.04.19 | 誤記修正 |
| 1.00.B1 | 2024.03.21 | </br>- 対応ボードにNUCLEO-L4R5ZIを追加。関連情報の記載</br>-「2. BSP固有の機能について」説明の追加．デバイスドライバ名の変更などの内容を更新 |
| 1.00.B0 | 2023.12.15 | 新規作成 |
