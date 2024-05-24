# μT-Kernel 3.0 BSP2 ユーザーズマニュアル <!-- omit in toc -->
## NXP MCUXpresso編 <!-- omit in toc -->
## Version 01.00.B4 <!-- omit in toc -->
## 2024.05.24 <!-- omit in toc -->

- [1. 概要](#1-概要)
  - [1.1. 対象マイコンボード](#11-対象マイコンボード)
  - [1.2. 開発環境](#12-開発環境)
  - [1.3. ソフトウェアの構成](#13-ソフトウェアの構成)
- [2. BSP固有の仕様および機能](#2-bsp固有の仕様および機能)
  - [2.1. スタックポインタ監視機能](#21-スタックポインタ監視機能)
  - [2.2. デバッグ用シリアル出力](#22-デバッグ用シリアル出力)
  - [2.3. 標準ヘッダファイルの使用](#23-標準ヘッダファイルの使用)
  - [2.4. FPU対応](#24-fpu対応)
- [3. デバイスドライバ](#3-デバイスドライバ)
  - [3.1. A/DCデバイスドライバ](#31-adcデバイスドライバ)
    - [3.1.1. 概要](#311-概要)
    - [3.1.2. デバイスドライバの使用方法](#312-デバイスドライバの使用方法)
  - [3.2. I2Cデバイスドライバ](#32-i2cデバイスドライバ)
    - [3.2.1. 概要](#321-概要)
    - [3.2.2. デバイスドライバの使用方法](#322-デバイスドライバの使用方法)
- [4. プログラムの作成手順](#4-プログラムの作成手順)
  - [4.1. MCUXpresso IDEによるプロジェクト作成と設定](#41-mcuxpresso-ideによるプロジェクト作成と設定)
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
    - [4.4.2. プログラムの実行](#442-プログラムの実行)
- [5. 変更履歴](#5-変更履歴)


# 1. 概要
本書はμT-Kernel 3.0 BSP2の使用法について説明します。  
μT-Kenrel 3.0 BSP2は、マイコン・メーカなどから提供されるマイコンの開発環境、ツールを使用し、また同様に提供されるファームウェアなどを活用してリアルタイムOS μT-Kernel 3.0を使用するためのBSP(Board Support Package)です。  

本書ではNXPのマイコンを搭載したマイコンボードのμT-Kernel 3.0 BSP2について説明します。

## 1.1. 対象マイコンボード
μT-Kenrel 3.0 BSP2は以下のNXPのマイコンボードに対応しています。

| マイコンボード      | マイコン                      | CPUコア          | 備考                    |
| ------------ | ------------------------- | -------------- | --------------------- |
| FRDM-MCXN947 | MCX N947 (MCXN947VDF)     | Arm Cortex-M33 | MCX N94 MCU用FRDM開発ボード |
| LPC55S69-EVK | LPC55S69 (LPC55S69JBD100) | Arm Cortex-M33 | LPCXpresso55S69開発ボード  |

## 1.2. 開発環境
開発環境は、NXPの統合開発環境MCUXpresso IDEを使用します。
また、ファームウェアはとMCUXpresso SDKを使用します。  
本書では以下のバージョンで動作を確認しています。  

`MCUXpresso IDE v11.9.0 [2024-01-05]`  
`MCUXpresso SDK version ksdk2_0`  

詳しくは以下のWebサイトをご覧ください。

https://mcuxpresso.nxp.com  

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
    - nxp_mcux NXP MCUXpresso用のソースコード
      - CPU CPU(ARMコア)依存部のソースコード
      - lib ライブラリのハードウェア依存部
      - device　サンプル・デバイスドライバ
      - doc   ドキュメント

mtkernelディレクトリは、トロンフォーラムから公開されているμT-Kernel 3.0のGitサブモジュールです。なお、mtkernelディレクトリ下のソースコードは、ハードウェアに依存しない共通部のコードのみを使用しています。  
マイコンボードやファームウェアに依存するソースコードはsysdependディレクトリにあります。  
deviceディレクトリにはMCUXpresso SDKを利用したデバイスドライバのサンプル・プログラムがあります。I2CおよびA/DCの基本的な機能が使用可能です。  

# 2. BSP固有の仕様および機能
## 2.1. スタックポインタ監視機能
Arm Cortex-M33のCPUスタックポインタモニタの機能を使用し、タスクのスタックのオーバフローを監視しています。スタックオーバーフローが検出されると、UsageFault例外が発生します。  
この機能はBSPコンフィギュレーションファイル (config/config_bsp/nxp_mcux/config_bsp.h) の以下を変更しビルドすることにより、有効無効を切り替えられます。  

```C
/*
 *  Stack pointer monitoring function
 */
 #define USE_SPMON		(1)		// 1:Valid   0:invalid
```

## 2.2. デバッグ用シリアル出力
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
デバッグ用シリアル出力はMCUXpresso IDEのデバッグ出力(PRINTF)を使用して実現しています。よって出力先はIDEのデバッグ出力の設定と同じとなります。出力先をデバッグ用のUSBとした場合は、シリアル出力は以下の設定となります。  

| 項目       | 値         |
| -------- | --------- |
| 速度       | 115200bps |
| データ長     | 8bit      |
| パリティ     | なし        |
| Stop Bit | 1bit      |
| フロー制御    | 無し        |

## 2.3. 標準ヘッダファイルの使用
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

## 2.4. FPU対応
本BSPはマイコン内蔵のFPUに対応しています。タスクのコンテキスト情報にFPUのレジスタなどの情報を含めます。  
コンフィギュレーションファイル(config/config.h)の以下を変更することにより、FPUの使用・未使用が指定できます。ただし、SDKはFPUを有効にしますので、本BSPのFPU対応は常に有効にする必要があります。  

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
## 3.1. A/DCデバイスドライバ
### 3.1.1. 概要
A/DCデバイスドライバは、マイコン内蔵のA/Dコンバータを制御することができます。 
本BSPでは以下のデバイスに対応したA/DCデバイスドライバがあります。  

| デバイス名 | BSPのデバイス名 | 説明                          |
| ------- | --------- | --------------------------- |
| ADC     | hadc      | Analog-to-Digital Converter |

A/DCデバイスドライバは内部の処理でMCUXpresso SDKのLPADCドライバを使用します。本デバイスドライバはMCUXpresso SDKのドライバをμT-Kernel 3.0 で使用する方法を示すサンプルプログラムであり、デバイスの基本的な機能のみに対応しています。  
以下にA/DCデバイスドライバのソースコードがあります。  

```mtk3_bsp2\sysdepend\nxp_mcux\device\hal_adc```

A/DCデバイスドライバはBSPコンフィギュレーションファイル (config/config_bsp/nxp_mcux/config_bsp.h) の以下を変更しビルドすることにより、使用・不使用を切り替えられます。

```C
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_ADC		1	// A/D conversion device
```

A/DCデバイスドライバはMCUXpresso SDKのLPADCドライバを使用していますので、LPADCドライバを使用可能にしてください。LPADCドライバと本デバイスドライバの関連付けは次項で説明します。  

### 3.1.2. デバイスドライバの使用方法
(1) A/Dコンバータ(ハードウェア)の設定  
A/DCデバイスドライバから使用するA/Dコンバータの設定をMCUXpresso IDEで行います。  
`Pins Configuration`でA/Dコンバータが使用する端子を設定します。  

(参考) 各ボードのArduino互換インタフェースのアナログ入力(A0～A5)とマイコンの端子の対応は以下の通りです。  

| Arduinoアナログ入力 | FRDM-MCXN947     | LPC55S69-EVK |
| ------------- | ---------------- | ------------ |
| A0            | ADC0_A0          | ADC0_8 (CH,0A)  |
| A1            | ADC0_B0          | ADC0_0 (CH,0B)  |
| A2            | ADC0_B14         |              |
| A3            | ADC0_A14         |              |
| A4            | ADC0_B15         |              |
| A5            | ADC0_A15         |              |

`Clocks Configuration`で使用するA/Dコンバータのクロック（ADC0CLKまたはADC1CLK）を有効にします。  

(2) LPADCドライバの設定  
`Peripheral Configuration`で、`Periphetal drivers` から `LPADC`を選択し、各項目を設定します。  
`General Configuration`はA/Dコンバータの設定を行います。  

| 項目              | 設定        |
| --------------- | --------- |
| VREFH souce | VDDA |
| その他 | 初期値(必要に応じて変更) |

`Conversion Commands Configuration`でアナログ入力信号のチャネルごとにCommandを作成します。本デバイスドライバでは以下の設定としてください。  

| 項目              | 設定        |
| --------------- | --------- |
| Channel smaple mode | Single end mode |
| Channnel number | 使用するチャンネル |
| Hardware average mode | Single conversion |
| その他             | 初期値（必要に応じて変更）    |

`Conversion triggers confiruration`で、各コマンドについてトリガーを作成します。本デバイスドライバでは以下の設定としてください。  

| 項目              | 設定        |
| --------------- | --------- |
| Trigger channnel number | 対象とするCommand number |
| その他             | 初期値（必要に応じて変更）    |

`Interrupt confiruration`で、割込みの設定を行います。本デバイスドライバでは以下の設定としてください。  

| 項目              | 設定        |
| --------------- | --------- |
| Intterrupt sources | FIFO 0 watermark |
| Enable interrupt vector | チェック |
| Priority | 2～6 |
| その他             | 初期値のまま    |

複数のA/Dコンバータを使用する場合は、必要な数だけ上記を繰り返します。  
設定後に`Update Code`を押下するとLPADCなどのコードが生成されます。  

(3) A/DCデバイスドライバの初期化  
A/DCデバイスドライバを使用するにあたり、最初に`dev_init_hal_adc`関数で初期化を行います。これにより、指定したLPADCドライバが関連付けられたA/DCデバイスドライバが生成されます。本関数は以下のように定義されます。  

```C
ER dev_init_hal_adc(
    UW unit,         // デバイスのユニット番号(0～DEV_HAL_ADC_UNITNM)
    ADC_Type *base   // A/Cコンバータのベースアドレス
)；
```

パラメータunitは0から順番に指定します。数字を飛ばすことはできません。  
パラメータbaseは、LPADCドライバを設定すると生成されるA/Dコンバータのベースアドレス情報です。  
初期化に成功するとデバイス名`hadc*`のデバイスドライバが生成されます。デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hadca`、ユニット番号1のデバイス名は`hadcb`となります。  

μT-Kernel 3.0 BSP2の起動処理の`knl_start_device`関数にてデバイスドライバの初期化を行っています。knl_start_device関数は以下のファイルに記述されています。  

`mtk3_bsp2/sysdepend/nxp_mcux/devinit.c`

以下にknl_start_device関数の内容を示します。ここではADC0のA/Dコンバータを使用したLPADCドライバを関連付けたA/DCデバイスドライバ`hadca`が生成されます。  
実際に使用するLPADCに応じて変更してください。  

```C
EXPORT ER knl_start_device( void )
{
	ER	err	= E_OK;

        // 一部省略

#if DEVCNF_USE_HAL_ADC
	err = dev_init_hal_adc( 0, ADC0);
#endif

	return err;
}
```

(4) デバイスドライバの操作  
μT-Kernel 3.0のデバイス管理APIにより、A/DCデバイスドライバを操作できます。APIの詳細はμT-Kernel 3.0仕様書を参照してください。    
最初にオープンAPI tk_opn_devにて対象とするデバイス名を指定しデバイスをオープンします。  
オープン後は同期リードAPI tk_srea_devによりデータを取得することができます。パラメータのデータ開始位置に設定したLPADCのトリガー番号を指定します。  
リードするデータ数は必ず1としてください。本デバイスドライバは、一度のアクセスで一つのチャンネルから1データのみを取得するように作られています。  

以下にA/DCデバイスドライバを使用したサンプル・プログラムを示します。  
このプログラムは500ms間隔でA/Dコンバータのトリガー0とトリガー1のチャンネルからデータを取得し、その値をデバッグ用シリアル出力に送信するタスクの実行関数です。トリガーの番号は実際に使用するA/Dコンバータに合わせてください。    

```C
LOCAL void task_1(INT stacd, void *exinf)
{
  UW	adc_val1, adc_val2;
	ID	dd;     // デバイスディスクリプタ
  SZ  asz;
	ER	err;    // エラーコード

	dd = tk_opn_dev((UB*)"hadca", TD_UPDATE);    // デバイスのオープン
	while(1) {
		err = tk_srea_dev(dd, 0, &adc_val1, 1, &asz);   // A/DC トリガー0からデータを取得
		err = tk_srea_dev(dd, 1, &adc_val2, 1, &asz);   // A/DC トリガー1からデータを取得
		tm_printf((UB*)"A/DC A0 =%06d A/DC A1 =%06d\n", adc_val1, adc_val2);    // デバッグ出力
		tk_dly_tsk(500);       // 500ms待ち
	}
}
```

## 3.2. I2Cデバイスドライバ
### 3.2.1. 概要
I2Cデバイスドライバは、マイコン内蔵のI2C通信デバイスを制御することができます。 
本BSPでは以下のデバイスに対応したI2Cデバイスドライバがあります。  

| デバイス名     | BSPのデバイス名 | 説明                                                 |
| ----------- | --------- | -------------------------------------------------- |
| LP_FLEXCOMM | hiic      | Low-Power Flexible Communications Interface のLPI2C |

I2Cデバイスドライバは内部の処理でMCUXpresso SDKのLPI2Cドライバを使用します。本デバイスドライバはMCUXpresso SDKのドライバをμT-Kernel 3.0 で使用する方法を示すサンプルプログラムであり、デバイスの基本的な機能のみに対応しています。  
以下にI2Cデバイスドライバのソースコードがあります。  

```mtk3_bsp2/sysdepend/nxp_mcux/device/hal_lpi2c```

I2CデバイスドライバはBSPコンフィギュレーションファイル (config/config_bsp/nxp_mcux/config_bsp.h) の以下を変更しビルドすることにより、使用・不使用を切り替えられます。  

```C
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_LPI2C		1	// I2C communication device (Use LP_FLEXCOMM )
```
I2CデバイスドライバはMCUXpresso SDKのLPI2Cドライバを使用していますので、LPI2Cドライバを使用可能にしてください。LPI2Cドライバと本デバイスドライバの関連付けは次項で説明します。  

### 3.2.2. デバイスドライバの使用方法
(1) LPI2C(ハードウェア)の設定  
I2Cデバイスドライバから使用するLPI2Cの設定をMCUXpresso IDEで行います。  
`Pins Configuration`でLPI2Cが使用する端子を設定します。  

(参考) 各ボードのArduino互換インタフェースのI2C信号とマイコンの端子の対応は以下の通りです。  

| Arduino I2C信号 | FRDM-MCXN947          |
| ------------- | ----------------------- |
| D15/SCL       | LP_FLEXCOMM2 SCL (P4_1) |
| D14/SDA       | LP_FLEXCOMM2 SDA (P4_0) |


`Clocks Configuration`で使用するLP_FLECCOMMのクロック（FLEXCOMM*CLK）を有効にします。  

(2) LPI2Cドライバの設定  
`Peripheral Configuration`で、`Periphetal drivers` から `LP_FLEXCOMM LPI2C`を選択し、各項目を設定します。  

| 項目                             | 設定                                                         |
| ------------------------------ | ---------------------------------------------------------- |
| Mode                           | Interrupt                                                  |
| LPI2C mode                     | Master mode                                                |
| Master interrupt sources       | Trinsmit data, Receive data, Stop detect, NACK detectにチェック |
| Enable interrupt vector        | チェック                                                       |
| Interrupt request              | No initialized                                             |
| Enable priority initialization | チェック                                                       |
| Priority                       | 2～6                                                        |
| その他                            | 初期値（必要に応じて設定）                                              |

複数のLPI2Cを使用する場合は、必要な数だけ上記を繰り返します。  
設定後に`Update Code`を押下するとLPI2Cなどのコードが生成されます。  

(3) I2Cデバイスドライバの初期化  
I2Cデバイスドライバを使用するにあたり、最初に`dev_init_hal_lpi2c`関数で初期化を行います。これにより、指定したLPI2Cドライバが関連付けられたI2Cデバイスドライバが生成されます。本関数は以下のように定義されます。  

```C
ER dev_init_hal_lpi2c(
      UW unit,         // デバイスのユニット番号(0～DEV_HAL_LPI2C_UNITNM)
      LPI2C_Type *base // LPI2Cのベースアドレス
);
```

パラメータunitは0から順番に指定します。数字を飛ばすことはできません。  
パラメータbaseは、LPI2Cドライバを設定すると生成されるLPI2Cのベースアドレス情報です。  
初期化に成功するとデバイス名`hiic*`のデバイスドライバが生成されます。デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hiica`、ユニット番号1のデバイス名は`hiicb`となります。 

μT-Kernel 3.0 BSP2の起動処理の`knl_start_device`関数にてデバイスドライバの初期化を行っています。knl_start_device関数は以下のファイルに記述されています。   

`mtk3_bsp2/sysdepend/nxp_mcux/devinit.c`

以下にknl_start_device関数の内容を示します。ここではLP_FLEXCOMM2を使用したLPI2Cドライバを関連付けたI2Cデバイスドライバ`hiica`が生成されます。  
実際に使用するLPI2Cに応じて指定を変更してください。  

```C
ER knl_start_device( void )
{
	ER	err	= E_OK;

#if DEVCNF_USE_HAL_LPI2C
	err = dev_init_hal_lpi2c( 0, LPI2C2);
	if(err < E_OK) return err;
#endif

        // 以下省略
}
```

(4) デバイスドライバの操作  
μT-Kernel 3.0のデバイス管理APIにより、デバイスドライバを操作できます。APIの詳細はμT-Kernel 3.0仕様書を参照してください。    
本デバイスドライバは、I2Cのコントローラ(マスター)モードにのみ対応します。  
最初にオープンAPI tk_opn_devにて対象とするデバイス名を指定しデバイスをオープンします。  
オープン後は同期リードAPI tk_srea_devによりデータの受信、同期ライトAPI tk_swri_devによりデータの送信を行うことができます。パラメータのデータ開始位置に対象のターゲット(スレーブ)アドレスを指定します。  

(5) デバイスのレジスタアクセス  
I2Cで接続されたターゲットデバイス内のレジスタをアクセスするための以下の関数が用意されています。これは比較的によく使用されるデバイス内のレジスタアクセス手順に対応しています。ただし、すべてのデバイスに使用できるわけではありませんので注意してください。

レジスタリード関数`hal_lpi2c_read_reg`は、ターゲットデバイスに対して、レジスタアドレスの値(1byte)を送信し、続いてデータ(1byte)の受信を行います。  

```C
/* レジスタリード関数 */
ER hal_lpi2c_read_reg(
    ID dd,    // デバイスディスクリプタ
    UW sadr,  // ターゲットアドレス
    UW radr,  // レジスタアドレス (下位8bitのみ有効)
    UB *data  // リードしたデータ(1byte)
);
```
レジスタライト関数`hal_lpi2c_write_reg`は、ターゲットデバイスに対して、レジスタアドレスの値(1byte)を送信し、続いてデータ(1byte)の送信を行います。  

```C
/* レジスタライト関数 */
ER hal_lpi2c_write_reg(    // SCI_I2Cの場合はhal_sci_i2c_write_reg
    ID dd,    // デバイスディスクリプタ
    UW sadr,  // ターゲットアドレス
    UW radr,  // レジスタアドレス (下位8bitのみ有効)
    UB data  // ライトするデータ(1byte)
);
```

# 4. プログラムの作成手順
MCUXpresso IDEでプログラムのプロジェクトを作成し、μT-Kenrel 3.0 BSP2を組み込んでビルド、実行までの手順を説明します。  
MCUXpresso IDEには対象とするマイコンボードのMCUXpresso SDKがインストールされていることが前提です。  

## 4.1. MCUXpresso IDEによるプロジェクト作成と設定
(1) プロジェクトの作成
以下の手順で対象マイコンボードのプロジェクトを作成します。  

 - メニュー[File]→[New]]→[Create New C/C++ Project]を選択します。  
 - 対象のマイコンボードを選択し、[Next]を押下します。  
 - [Configure th project]のページにて[Project name]に任意の名称を設定します。[Components]から[Drivers]→[Device]→[SDK Drivers]と進み、使用するMCUXpresso SDKのデバイスドライバを選択し、 [Next]を押下します。  
 - [Advanced project setting]のページで必要に応じて設定を行い[Finish]を押下します。

以上でプロジェクトが生成され、基本的なソースコードなどが生成されます。  

(2) プロジェクトの設定
メニュー[ConfigTools]から以下の設定を行います。  

 - [Pins] デバイスドライバから使用する端子などの設定を行います。  
 - [Clocks]  CPUやデバイスへ供給するクロックの設定を行います。
 - [Peripherals] MCUXpresso SDKのデバイスドライバの選択、設定などを行います。

設定を終えたのち`Update Code`を押下すると、設定に応じたコードが生成されます。  

## 4.2. μT-Kernel 3.0 BSP2の組込み
### 4.2.1. ソースコードの組込み
作成したプロジェクトにμT-Kernel 3.0 BSP2のソースコードを組込みます。  
以下のGitHubのリポジトリからμT-Kernel 3.0 BSP2から入手し、プロジェクトのディレクトリの中にμT-Kernel 3.0 BSP2のソースコードのディレクトリ`mtk3_bsp2`を置きます。

`https://github.com/tron-forum/mtk3_bsp2.git`  

gitのコマンドを使用する場合は、プロジェクトのディレクトリをカレントディレクトリとし、以下のコマンドを実行します。  

`git clone --recursive  https://github.com/tron-forum/mtk3_bsp2.git`  

μT-Kernel 3.0 BSP2はμT-Kernel 3.0のリポジトリをgitのサブモジュールとして内包していますので、全ソースコードを取得するには--recursiveが必要となります。  

取り込んだμT-Kernel 3.0 BSP2のディレクトリは、MCUXpresso IDEのビルド対象外の設定になっている可能性がありますので、プロパティを変更します。  
`Exclude resource from build`にチェックが入っている場合は外してください。  

### 4.2.2. ビルド設定の追加
プロジェクトのプロパティに、μT-Kernel 3.0 BSP2のスースコードをビルドするための設定を追加します。  
[C/C++ Build]→[Settiongs]→[Tool Settiongs]で以下を設定します。

(1) [MCUs C Compiler]→[Preprocessor]  
[Define symbles]に対象マイコンボードのターゲット名を設定します。

| マイコンボード      | ターゲット名                  |
| ------------ | ----------------------- |
| FRDM-MCXN947 | \_NXPMCUX_FRDM_MCXN947_ |
| LPC55S69-EVK | \_NXPMCUX_EVK_LPC55S69_ |


(2) [MCU C Compiler]→[includes]  
[Include paths]に以下を設定します。  
プロジェクトのディレクトリ・ツリーの最上位にμT-Kernel 3.0 BSP2のディレクトリmtk3_bsp2がある前提です。ディレクトリの場所を変えた場合はそれに合わせて変更してください。  

```
"${workspace_loc:/${ProjName}/mtk3_bsp2}"
"${workspace_loc:/${ProjName}/mtk3_bsp2/config}"
"${workspace_loc:/${ProjName}/mtk3_bsp2/include}"
"${workspace_loc:/${ProjName}/mtk3_bsp2/mtkernel/kernel/knlinc}"
```

(3) [MCU Assembler]→[General]
[[Assembler flags]に以下を追加します。  

| マイコンボード      | 追加する指定                    |
| ------------ | ------------------------- |
| FRDM-MCXN947 | \-D_NXPMCUX_FRDM_MCXN947_ |
| LPC55S69-EVK | \-D_NXPMCUX_EVK_LPC55S69_ |

(4) [MCU Assembler]→[General]  
[Include paths]に(2)と同じ設定をします。  

### 4.2.3. OS起動処理の呼び出し
生成されたプロジェクトは、ハードウェアの初期化処理などのあと、main関数を実行します。  
main関数からμT-Kernel 3.0の起動処理関数`knl_start_mtkernel`を実行するようにコードを追加します。  
main関数は以下のファイルの記述されています。

`<プロジェクトのディレクトリ>/source/<プロジェクト名>.c`

main関数に以下のように記述します。

```C
int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /* ここから追加 */
    void knl_start_mtkernel(void);
    knl_start_mtkernel();         // μT-Kernel 3.0 の起動
    /* ここまで */

    return 0 ;
}
```

## 4.3. ユーザプログラム
### 4.3.1. ユーザプログラムの作成
μT-Kernel 3.0で実行するユーザプログラムを作成します。  
ユーザプログラムを作成するディレクトリは名前も場所も任意です。たとえば、プロジェクトのディレクトリ・ツリーの最上位にapplicationなどといった名称のディレクトリを作成します。  

### 4.3.2. usermain関数
μT-Kernel 3.0は起動するとユーザプログラムのusermain関数を実行します。よってusermain関数を定義します。  
usermain関数は以下の形式です。

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
プロジェクトを選択し、メニュー[Project]→[Build Project]または右クリックメニューにより、プロジェクトをビルドします。  
エラーが発生せずに正常に終了すれば実行プログラム(ELFファイル)が生成されています。  

### 4.4.2. プログラムの実行
マイコンボードをPCに接続し、MCUXpresso IDEの[Quickstart Panel]の[Debug your project]からデバッグ実行を行うことができます。  
また、メニュー[Run]→[Debug As]を選択すると、以前にデバッグ実行した実行プログラムを再びデバッグ実行します。  
実行プログラムはマイコンボードのフラッシュメモリーに書き込まれますので、デバッガを外した状態でマイコンボードの電源を入れれば、そのまま実行されます。  

# 5. 変更履歴

| 版数      | 日付         | 内容   |
| ------- | ---------- | ---- |
| 1.00.B4 | 2024.05.24 | 誤記修正 |
| 1.00.B3 | 2024.05.16 | 「2.2. デバッグ用シリアル出力」機能追加 |
| 1.00.B2 | 2024.04.12 | 「4.2.2. ビルド設定」　変更、誤字修正 |
| 1.00.B1 | 2024.03.21 | 誤記修正 |
| 1.00.B0 | 2024.02.29 | 新規作成 |
