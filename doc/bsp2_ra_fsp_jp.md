# μT-Kernel 3.0 BSP2 ユーザーズマニュアル <!-- omit in toc -->
## RA FSP編 <!-- omit in toc -->
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
  - [4.1. e2studioによるプロジェクト作成](#41-e2studioによるプロジェクト作成)
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


# 1. 概要
本書はμT-Kernel 3.0 BSP2の使用法について説明します。  
μT-Kenrel 3.0 BSP2は、マイコン・メーカなどから提供されるマイコンの開発環境、ツールを使用し、また同様に提供されるファームウェアなどを活用してリアルタイムOS μT-Kernel 3.0を使用するためのBSP(Board Support Package)です。  

本書ではルネサス エレクトロニクス株式会社のマイコンRAを搭載したマイコンボードのμT-Kernel 3.0 BSP2について説明します。

## 1.1. 対象マイコンボード
μT-Kenrel 3.0 BSP2は以下のRAマイコンボードに対応しています。

| マイコンボード        | マイコン                 | CPUコア         | 備考  |
| -------------- | -------------------- | ------------- | --- |
| EK-RA6M3       | RA6M3(R7FA6M3AH3CFC) | Arm Cortex-M4 |ルネサス エレクトロニクス RA6M3 MCUグループ評価キット     |
| EK-RA8M1       | RA8M1(R7FA8M1AHECBD) | Arm Cortex-M85 |ルネサス エレクトロニクス RA8M1 MCUグループ評価キット     |
| Arduino UNO R4 | RA4M1(R7FA4M1AB3CFM) | Arm Cortex-M4 |     |

## 1.2. 開発環境
開発環境は、ルネサス エレクトロニクス株式会社の統合開発環境e2studioを使用します。  
また、ファームウェアとして、FSP(Flexible Software Package )を使用します。  
本書では以下のバージョンで動作を確認しています。  

`Renesas e² studio Version: 2023-10 (23.10.0)`  
`FSP version 5.1.0`  

詳しくは以下のWebサイトをご覧ください。

統合開発環境 e² studio RAファミリ向け情報  
https://www.renesas.com/jp/ja/software-tool/e2studio-information-ra-family  

FSP(Flexible Software Package )  
https://www.renesas.com/jp/ja/software-tool/flexible-software-package-fsp  

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
    - ra_fsp  RAマイコンおよびFSP用のソースコード
      - CPU CPU(ARMコア)依存部のソースコード
      - lib ライブラリのハードウェア依存部
      - device　サンプル・デバイスドライバ
      - doc   ドキュメント

mtkernelディレクトリは、トロンフォーラムから公開されているμT-Kernel 3.0のGitサブモジュールです。なお、mtkernelディレクトリ下のソースコードは、ハードウェアに依存しない共通部のコードのみを使用しています。  
マイコンボードやファームウェアに依存するソースコードはsysdependディレクトリにあります。  
deviceディレクトリにはFSPを利用したデバイスドライバのサンプル・プログラムがあります。I2CおよびA/DCの基本的な機能が使用可能です。  

# 2. BSP固有の仕様および機能
## 2.1. スタックポインタ監視機能
RAマイコンのCPUスタックポインタモニタの機能を使用し、タスクのスタックのオーバフローを監視しています。  
この機能はBSPコンフィギュレーションファイル (config/config_bsp/ra_fsp/config_bsp.h) の以下を変更しビルドすることにより、有効無効を切り替えられます。  

```C
/*
 *  Stack pointer monitoring function
 */
 #define USE_SPMON		(1)		// 1:Valid   0:invalid
```

スタックポインタ監視機能は、マイコンのCPUコアにより動作が異なります。  

(1) Arm Cortex-M4コアの場合  
RAマイコン独自の機能によりスタックポインタを監視しています。スタックオーバーフローが検出されると、NMI例外が発生します。  

(2) Arm Cortex-M85コアの場合  
ARMv8-Mの機能によりスタックポインタを監視しています。スタックオーバーフローが検出されると、UsageFault例外が発生します。  

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
シリアル出力は以下の設定となります。  

| 項目       | 値         |
| -------- | --------- |
| 速度       | 115200bps |
| データ長     | 8bit      |
| パリティ     | なし        |
| Stop Bit | 1bit      |
| フロー制御    | 無し        |

シリアル信号はボードのArduino互換コネクタのTXおよびRXを使用します。本機能を使用する場合は、FSPのコンフィギュレーションにより、これらの端子を`Peripheral mode`に設定します。
またペリフェラルへの供給クロックの設定を行ってください。低消費電力モードの解除は本機能の初期化で行いますので必要ありません。  
以下にデバッグ用シリアル出力で使用するマイコンの端子を示します。  

| マイコンボード        | TX端子       | RX端子       |
| -------------- | ---------- | ---------- |
| EK-RA6M3       | P613(TXD7) | P614(RXD7) |
| EK-RA8M1       | P310(TXD3) | P309(RXD3) |
| Arduino UNO R4 | P302(TXD2) | P301(RXD2) |


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
## 3.1. A/DCデバイスドライバ
### 3.1.1. 概要
A/DCデバイスドライバは、マイコン内蔵のA/Dコンバータを制御することができます。  
本BSPでは以下のデバイスに対応したA/DCデバイスドライバがあります。  

| デバイス名 | BSPのデバイス名 | 説明                  |
| ----- | --------- | ------------------- |
| ADC12 | hadc      | 12bit A/C Converter |

デバイスドライバは内部の処理でFSPのHALを利用します。本デバイスドライバはFSPののHALをμT-Kernel 3.0 で使用する方法を示すサンプルプログラムであり、デバイスの基本的な機能のみに対応しています。  
以下にA/DCデバイスドライバのソースコードがあります。  

```mtk3_bsp2\sysdepend\ra_fsp\device\hal_adc```

このデバイスドライバはBSPコンフィギュレーションファイル (config/config_bsp/ra_fsp/config_bsp.h) の以下を変更しビルドすることにより、使用・不使用を切り替えられます。

```C
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_ADC		1	// A/D conversion device
```

A/DCデバイスドライバはFSPのHALを使用していますので、A/DCのHALを使用可能にしてください。HALと本デバイスドライバの関連付けは次項で説明します。  

### 3.1.2. デバイスドライバの使用方法
(1) A/Dコンバータ(ハードウェア)の設定  
A/DCデバイスドライバから使用するA/Dコンバータの設定をe2studioで行います。  
`Pin Configuration`で使用する端子の`Mode`を`Analog mode`に設定します。  

(参考) 各ボードのArduino互換インタフェースのアナログ入力(A0～A5)と、マイコンのA/Dコンバータの入力の対応は以下の通りです。  

| Arduinoアナログ入力 | EK-RA6M3   | EK-RA8M1    | Arduino UNO R4 |
| ------------- | ---------- | ----------- | -------------- |
| A0            | P000/AN000 | P004(AN000) | P014/AN009     |
| A1            | P001/AN001 | P003(AN104) | P000/AN000     |
| A2            | P002/AN002 | P007(AN004) | P001/AN001     |
| A3            | P507/AN119 | P001(AN101) | P002/AN002     |
| A4            | P508/AN020 | P014(AN007) | P101/AN021     |
| A5            | P014/AN005 | P015(AN105) | P100/AN022     |

(2) HALの設定
`Stacks Configuration`で、`New Stack` → `Analog` → `ADC(r_adc)`を選択し、プロパティの`Module g_adc0 ADC(r_adc)`の各項目を以下のように設定します。  

| 分類        | 項目                          | 設定                  |
| --------- | --------------------------- | ------------------- |
| General   | Name                        | g_adc0 (任意の名称に変更可能) |
|           | Unit                        | 使用するA/Dコンバータのユニット番号 |
|           | その他                         | 初期値のまま              |
| Input     | Channel Scan Mask           | 使用するチャンネルにチェック      |
|           | その他                         | 初期値のまま              |
| Interrupt | Scan End Interrupt Priority | 任意の割込み優先度           |
|           | その他                         | 初期値のまま              |

複数のA/Dコンバータを使用する場合は、必要な数だけ上記を繰り返します。
設定後に`Genetate Project Content`を押下するとFSP(HAL)のコードが生成されます。  

(3) デバイスドライバの初期化  
A/DCデバイスドライバを使用するにあたり、最初に`dev_init_hal_adc`関数で初期化を行います。これにより、HALが関連付けられたA/DCデバイスドライバが生成されます。本関数は以下のように定義されます。  

```C
ER dev_init_hal_adc(
    UW unit,        // デバイスのユニット番号(0～DEV_HAL_ADC_UNITNM)
    adc_instance_ctrl_t *hadc,      // ADC制御構造体
	  const adc_cfg_t *cadc,          // ADCコンフィギュレーション構造体
    const adc_channel_cfg_t *cfadc  // ADCチャンネルコンフィギュレーション構造体
);
```

パラメータunitは0から順番に指定します。数字を飛ばすことはできません。  
パラメータhadc, cadc, cfadcは、FSPを設定すると自動的に生成されるHALの情報です。  
初期化に成功するとデバイス名`hadc*`のデバイスドライバが生成されます。デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hadca`、ユニット番号1のデバイス名は`hadcb`となります。  

μT-Kernel 3.0 BSP2の起動処理の`knl_start_device`関数にてデバイスドライバの初期化を行っています。knl_start_device関数は以下のファイルに記述されています。  

`mtk3_bsp2/sysdepend/ra_fsp/devinit.c`

以下にknl_start_device関数の内容を示します。ここでは名称`g_adc0`のFSP(HAL)を使用したデバイスドライバを初期化し、デバイス名`hadca`のμT-Kernel 3.0のデバイスドライバが生成されます。  
実際に使用するFSP(HAL）に応じて変更してください。  

```C
EXPORT ER knl_start_device( void )
{
	ER	err	= E_OK;

        // 一部省略

#if DEVCNF_USE_HAL_ADC
	err = dev_init_hal_adc( 0, &g_adc0_ctrl, &g_adc0_cfg, &g_adc0_channel_cfg);
#endif

	return err;
}
```

(4) デバイスドライバの操作  
μT-Kernel 3.0のデバイス管理APIにより、デバイスドライバを操作できます。APIの詳細はμT-Kernel 3.0仕様書を参照してください。    
最初にオープンAPI tk_opn_devにて対象とするデバイス名を指定しデバイスをオープンします。  
オープン後はリード同期リードAPI tk_srea_devによりデータを取得することができます。パラメータのデータ開始位置にA/Dコンバータのチャンネルを指定します。  
本デバイスドライバでは、一度のアクセスで一つのチャンネルから1データのみを取得できます。  

以下にA/DCデバイスドライバを使用したサンプル・プログラムを示します。  
このプログラムは500ms間隔でA/Dコンバータのチャンネル0とチャンネル1からデータを取得し、その値をデバッグ用シリアル出力に送信するタスクの実行関数です。チャンネルの番号は実際に使用するA/Dコンバータに合わせてください。  

```C
LOCAL void task_1(INT stacd, void *exinf)
{
  UW	adc_val1, adc_val2;
	ID	dd;     // デバイスディスクリプタ
	ER	err;    // エラーコード

	dd = tk_opn_dev((UB*)"hadca", TD_UPDATE);    // デバイスのオープン
	while(1) {
		err = tk_srea_dev(dd, 0, &adc_val1, 1, NULL);   // A/DC チャンネル0からデータを取得
		err = tk_srea_dev(dd, 1, &adc_val2, 1, NULL);   // A/DC チャンネル1からデータを取得
		tm_printf((UB*)"A/DC A0 =%06d A/DC A1 =%06d\n", adc_val1, adc_val2);    // デバッグ出力
		tk_dly_tsk(500);       // 500ms待ち
	}
}
```

## 3.2. I2Cデバイスドライバ
### 3.2.1. 概要
I2Cデバイスドライバは、マイコン内蔵のI2C通信デバイスを制御することができます。 
本BSPでは以下のデバイスに対応したI2Cデバイスドライバがあります。 

| デバイス名 | BSPのデバイス名 | 説明                               |
| ------- | --------- | -------------------------------- |
| IIC     | hiic      | IICバスインタフェース                     |
| SCI     | hsiic     | シリアルコミュニケーションインタフェースSCIの簡易IICモード |
| I3C | htiic | I3CバスインタフェースのI2Cバスインタフェース機能 |
  
デバイスドライバは内部の処理でFSPのHALを利用しています。本デバイスドライバはFSPのHALをμT-Kernel 3.0 で使用する方法を示すサンプルプログラムであり、デバイスの基本的な機能のみに対応しています。  
以下にI2Cデバイスドライバのソースコードがあります。

- I2C(IICを使用)  
```mtk3_bsp2/sysdepend/ra_fsp/device/hal_i2c```

- SCI_I2C(SCIを使用)  
```mtk3_bsp2/sysdepend/ra_fsp/device/hal_sci_i2c```

- I3C_I2C(I3Cを使用)  
```mtk3_bsp2/sysdepend/ra_fsp/device/hal_i3c_i2c```

このデバイスドライバはBSPコンフィギュレーションファイル (config/config_bsp/ra_fsp/config_bsp.h) の以下を変更しビルドすることにより、使用・不使用を切り替えられます。  
マイコンの種別により搭載されているI2C通信デバイスは異なります。搭載されていないI2C通信デバイスを使用する設定にしないでください。

```C
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_IIC      1	// I2C communication device (Use IIC )
#define DEVCNF_USE_HAL_SCI_IIC  1	// I2C communication device (Use SCI )
#define DEVCNF_USE_HAL_I3C_IIC  1	// I2C communication device (Use I3C )
```

I2CデバイスドライバはFPSのHALを使用していますので、I2CのHALを使用可能にしてください。HALと本デバイスドライバの関連付けは次項で説明します。  

### 3.2.2. デバイスドライバの使用方法
(1) I2C(ハードウェア)の設定  
I2Cデバイスドライバから使用するI2Cの設定をe2studioで行います。  
`Pin Configuration`で使用する端子のModeを`Peripheral mode`に設定します。  

(参考) 各ボードのI2C信号と、マイコンのI2C端子の対応は以下の通りです。  

| ボードのI2C信号  | EK-RA6M3      | EK-RA8M1      | Arduino UNO R4 |
| --------------- | ------------- | ------------- | -------------- |
| Grove-1 I2C SDA | P409/SCI3_SDA | P401/I3C_SDA0 | -              |
| Grove-1 I2C SCL | P408/SCI3_SCL | P400/I3C_SCL0 | -              |
| Grove-2 I2C SDA | P409/SCI3_SDA | P511/SDA1     | -              |
| Grove-2 I2C SCL | P408/SCI3_SCL | P512/SCL1     | -              |
| Arduino I2C SDA | P511/SDA2     | P401/I3C_SDA0 | P101/SDA1      |
| Arduino I2C SCL | P512/SCL2     | P400/I3C_SCL0 | P100/SCL1      |

**注意**  EK-RA8M1ボードではGrove-1およびArduinoのI2CインタフェースをI2Cとして使用する場合は、以下の端子の設定を行ってください。  

| 端子   | 設定                         |
| ---- | -------------------------- |
| P115 | Output mode (Initial Low)  |
| P711 | Output mode (Initial High) |
| PB00 | Output mode (Initial High) |

(2) HALの設定
`Stacks Configuration`で、`New Stack` → `Connectivity`から対象のI2Cペリフェラルを選択します。

- I2C(IICを使用) の場合  
I2C Master(r_iic_master)を選択し、プロパティの`Module g_i2c_master0`の各項目を以下のように設定します。  

| 項目                | 設定                         |
| ------------------ | -------------------------- |
| Name               | g_i2c_master0 (任意の名称に変更可能) |
| Unit               | 使用するI2Cのチャンネル番号            |
| Interrupt Priority | 任意の割込み優先度                  |
| その他              | 初期値のまま                     |

- SCI_I2C(SCIを使用)  
I2C Master(r_sci_i2c)を選択し、プロパティの`Module g_i2c0`の各項目を以下のように設定します。  

| 項目                | 設定                  |
| ------------------ | ------------------- |
| Name               | g_i2c0 (任意の名称に変更可能) |
| Unit               | 使用するI2Cのチャンネル番号     |
| Interrupt Priority | 任意の割込み優先度           |
| その他              | 初期値のまま              |

- I3C_I2C(I3Cを使用)  
I3C (r_i3c)を選択し、プロパティの`Module g_i3c`の各項目を以下のように設定します。  

| 項目                | 設定                   |
| ------------------ | -------------------- |
| Name               | g_i3c0  (任意の名称に変更可能) |
| Device Type        | Main Master          |
| Interrupt Priority | 任意の割込み優先度            |
| その他              | 初期値のまま               |

複数のI2Cを使用する場合は、必要な数だけ上記を繰り返します。  
設定後に`Genetate Project Content`を押下するとFSP(HAL)のコードが生成されます。  

(3) デバイスドライバの初期化  
I2Cデバイスドライバを使用するにあたり、最初にI2Cデバイスドライバ初期化関数で初期化を行います。これにより、指定したHALが関連付けられたI2Cデバイスドライバが生成されます。本関数は以下のように定義されます。  

- I2C(IICを使用) の場合
```C
ER dev_init_hal_i2c(
      UW unit,         // デバイスのユニット番号(0～DEV_HAL_ADC_UNITNM)
      i2c_master_ctrl_t       *hi2c,       // I2C制御構造体
    const i2c_master_cfg_t    *ci2c        // I2Cコンフィギュレーション構造体
);
```

- SCI_I2C(SCIを使用)  
```C
ER dev_init_hal_sci_i2c(
      UW unit,         // デバイスのユニット番号(0～DEV_HAL_ADC_UNITNM)
      i2c_master_ctrl_t       *hi2c,        // SCI_I2C制御構造体
      const i2c_master_cfg_t  *ci2c         // I2Cコンフィギュレーション構造体
);
```

- I3C_I2C(I3Cを使用)  
```C
ER dev_init_hal_i3c_i2c(
    UW unit,         // デバイスのユニット番号(0～DEV_HAL_ADC_UNITNM)
    i3c_instance_ctrl_t *hi3c,            // I3C制御構造体
    const i3c_cfg_t *ci3c                 // I3Cコンフィギュレーション構造体
);
```

パラメータunitは0から順番に指定します。数字を飛ばすことはできません。  
パラメータhai2c, ci2c（またはhai3c, ci3c）は、FSPを設定すると自動的に生成されるHALの情報です。 
初期化に成功するとデバイス名`hiic*`など(I2Cペリフェラルの種類により異なります)のデバイスドライバが生成されます。  
デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hiica`、ユニット番号1のデバイス名は`hiicb`となります。

μT-Kernel 3.0 BSP2の起動処理の`knl_start_device`関数にてデバイスドライバの初期化を行っています。knl_start_device関数は以下のファイルに記述されています。   

`mtk3_bsp2/sysdepend/ra_fsp/devinit.c`

以下にknl_start_device関数の内容を示します。ここではHALに関連図けられたμT-Kernel 3.0のデバイスドライバが生成されます。
実際に使用するFSP(HAL)に応じて指定を変更してください。  

```C
ER knl_start_device( void )
{
	ER	err	= E_OK;

#if DEVCNF_USE_HAL_IIC
	err = dev_init_hal_i2c(0, &g_i2c_master0_ctrl, &g_i2c_master0_cfg);
	if(err < E_OK) return err;
#endif

#if DEVCNF_USE_HAL_SCI_IIC
	err = dev_init_hal_sci_i2c(0, &g_i2c0_ctrl, &g_i2c0_cfg);
	if(err < E_OK) return err;
#endif

#if DEVCNF_USE_HAL_I3C_IIC
	err = dev_init_hal_i3c_i2c(0, &g_i3c0_ctrl, &g_i3c0_cfg);
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

レジスタリード関数は、ターゲットデバイスに対して、レジスタアドレスの値(1byte)を送信し、続いてデータ(1byte)の受信を行います。  
関数名はデバイスドライバの種類で異なります。  

| デバイスの種類 | 関数名                  |
| ------- | -------------------- |
| I2C     | hal_i2c_read_reg     |
| SCI_I2C | hal_sci_i2c_read_reg |
| I3C_I2C | hal_i3c_i2c_read_reg |

```C
/* レジスタリード関数 */
ER hal_***_read_reg(
    ID dd,    // デバイスディスクリプタ
    UW sadr,  // ターゲットアドレス
    UW radr,  // レジスタアドレス (下位8bitのみ有効)
    UB *data  // リードしたデータ(1byte)
);
```
レジスタライト関数は、ターゲットデバイスに対して、レジスタアドレスの値(1byte)を送信し、続いてデータ(1byte)の送信を行います。  
関数名はデバイスドライバの種類で異なります。  

| デバイスの種類 | 関数名                   |
| ------- | --------------------- |
| I2C     | hal_i2c_write_reg     |
| SCI_I2C | hal_sci_i2c_write_reg |
| I3C_I2C | hal_i3c_i2c_read_reg  |

```C
/* レジスタライト関数 */
ER hal_***_write_reg(    // SCI_I2Cの場合はhal_sci_i2c_write_reg
    ID dd,    // デバイスディスクリプタ
    UW sadr,  // ターゲットアドレス
    UW radr,  // レジスタアドレス (下位8bitのみ有効)
    UB data  // ライトするデータ(1byte)
);
```

# 4. プログラムの作成手順
e2studioでプログラムのプロジェクトを作成し、μT-Kenrel 3.0 BSP2を組み込んでビルド、実行までの手順を説明します。　　
e2studioにはRAマイコン用のFSPがインストールされていることが前提です。  

## 4.1. e2studioによるプロジェクト作成
以下の手順で対象マイコンボードのプロジェクトを作成します。  

(1) メニュー[ファイル]→[新規]→[Renesas C/C++ Project]→[Renesas RA]を選択します。   
(2) [Renesas RA C/C++ Project] を選択します。  
(3) 対象のマイコンボードを選択します。対象ボードが無い場合は`Custom User Board`を選択し、`Device`を指定します。  
Arduino UNO R4はボードの選択できませんので以下のように設定してください。  

- Board : Custom User Board
- Device : R7FA4M1AB3CFM
- クロックの設定  
  - HOCO : 48MHz
  - PLL Src : Disabled
  - Clock src : HOCO
- Subclock Populated: Not Populated

(4) [Project Template Selection]は`Bare Metal - Minimal`を選択します。  
(5) マイコンのピン設定やクロック、その他ハードウェアの設定を、開発するアプリケーションに応じて行ってください。  

## 4.2. μT-Kernel 3.0 BSP2の組込み
### 4.2.1. ソースコードの組込み
作成したプロジェクトにμT-Kernel 3.0 BSP2のソースコードを組込みます。  
以下のGitHubのリポジトリからμT-Kernel 3.0 BSP2から入手し、プロジェクトのディレクトリの中にμT-Kernel 3.0 BSP2のソースコードのディレクトリmtk3_bsp2を置きます。

`https://github.com/tron-forum/mtk3_bsp2.git`  

gitのコマンドを使用する場合は、プロジェクトのディレクトリをカレントディレクトリとし、以下のコマンドを実行します。  

`git clone --recursive  https://github.com/tron-forum/mtk3_bsp2.git`  

μT-Kernel 3.0 BSP2はμT-Kernel 3.0のリポジトリをgitのサブモジュールとして内包していますので、全ソースコードを取得するには--recursiveが必要となります。  

取り込んだμT-Kernel 3.0 BSP2のディレクトリは、e2studioのビルド対象外の設定になっている可能性がありますので、プロパティを変更します。  
`Exclude resource from build`にチェックが入っている場合は外してください。  

### 4.2.2. ビルド設定の追加
プロジェクトのプロパティに、μT-Kernel 3.0 BSP2のスースコードをビルドするための設定を追加します。  
[C/C++ ビルド]→[設定]→[ツール設定]で以下を設定します。

(1) [GNU Arm Cross C Compiler]→[Preprocessor]  
[Define symbles]に対象マイコンボードのターゲット名を設定します。

| マイコンボード        | ターゲット名                 |
| -------------- | ---------------------- |
| EK-RA6M3       | \_RAFSP_EK_RA6M3_      |
| EK-RA8M1       | \_RAFSP_EK_RA8M1_      |
| Arduino UNO R4 | \_RAFSP_ARDUINO_UNOR4_ |


(2) [GNU Arm Cross C Compiler]→[includes]  
[Include paths]に以下を設定します。  
プロジェクトのディレクトリ・ツリーの最上位にμT-Kernel 3.0 BSP2のディレクトリmtk3_bsp2がある前提です。ディレクトリの場所を変えた場合はそれに合わせて変更してください。  

```
"${workspace_loc:/${ProjName}/mtk3_bsp2}"
"${workspace_loc:/${ProjName}/mtk3_bsp2/config}"
"${workspace_loc:/${ProjName}/mtk3_bsp2/include}"
"${workspace_loc:/${ProjName}/mtk3_bsp2/mtkernel/kernel/knlinc}"
```

(3) [GNU Arm Cross Assembler]→[Preprocessor]  
(1)と同じ設定を行います。  

(4) [GNU Arm Cross Assembler]→[includes]  
(2)と同じ設定を行います。  

### 4.2.3. OS起動処理の呼び出し
生成されたプロジェクトは、ハードウェアの初期化処理などのあと、hal_entry関数を実行します。  
hal_entry関数からμT-Kernel 3.0の起動処理knl_start_mtkernel関数を実行するようにコードを追加します。
hal_entry関数は以下のファイルの記述されています。

`<プロジェクトのディレクトリ>/src/hal_entry.c`

hal_entry関数に以下のように記述します。

```C
void hal_entry(void)
{
    void knl_start_mtkernel(void);
    knl_start_mtkernel();
}
```

## 4.3. ユーザプログラム
### 4.3.1. ユーザプログラムの作成
μT-Kernel 3.0で実行するユーザプログラムを作成します。  
ユーザプログラムを作成するディレクトリは名前も場所も任意です。たとえば、プロジェクトのディレクトリ・ツリーの最上位にapplicationなどといった名称のディレクトリを作成します。  

### 4.3.2. usermain関数
μT-Kernel 3.0は起動するとユーザプログラムのusermain関数を実行します。よってusermain関数を定義します。  
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
プロジェクトを選択し、メニュー[プロジェクト]→[プロジェクトのビルド]または右クリックメニューにより、プロジェクトをビルドします。  
エラーが発生せずに正常に終了すれば実行プログラム(ELFファイル)が生成されています。

### 4.4.2. デバッグの設定
使用するデバッガに応じてデバッグの設定を行います。
デバッガはプロジェクトを生成する際にその種類を指定します（あとから変更も可能です）。RAマイコンではデバッガは`E2`、`E2 Lite`、`J-Link`の選択が可能です。マイコンボードにデバッガが搭載されている場合はそのデバッガの種類を指定します。
メニュー[実行]→[デバッグの構成]にてデバッグの各種設定をマイコンボードおよびデバッガに応じて行ってください。

### 4.4.3. プログラムの実行
マイコンボードをPCに接続し、メニュー[実行]→[デバッグの構成]を選び、設定を確認したのち、[デバッグ]ボタンをクリックすると、実行プログラムがマイコンボードに転送され、デバッグ実行が開始します。
また、メニュー[実行]→[デバッグ]を選択すると、前回デバッグ実行した実行プログラムを再びデバッグ実行します。
実行プログラムはマイコンボードのフラッシュメモリーに書き込まれますので、デバッガを外した状態でマイコンボードの電源を入れれば、そのまま実行されます。


# 5. 変更履歴

| 版数      | 日付         | 内容   |
| ------- | ---------- | ---- |
| 1.00.B4 | 2024.05.24 | 誤記修正 |
| 1.00.B3 | 2024.04.10 | I2Cデバイスの説明を補足 |
| 1.00.B2 | 2024.03.21 | 誤記修正 |
| 1.00.B1 | 2024.02.29 | </br>- 対応ボードにEK-RA8M1を追加。関連情報の記載</br>- デバイスドライバなどの内容を更新 |
| 1.00.B0 | 2023.12.15 | 新規作成 |
