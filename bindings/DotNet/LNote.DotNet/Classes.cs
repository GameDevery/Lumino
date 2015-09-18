﻿using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
    public partial class ote
    {
    
    
        /// <summary>
        /// デバッグ用のログファイルの出力有無を設定します。(初期値:false)
        /// </summary>
        /// <param name="enabled">true:出力する / false:出力しない</param>
        public static void SetApplicationLogEnabled( bool enabled)
        {
            API.LNConfig_SetApplicationLogEnabled( enabled);
        
        }
        
        /// <summary>
        /// 標準入出力用のコンソールウィンドウを割り当てるかどうかを設定します。(初期値:false)
        /// </summary>
        /// <param name="enabled">true:割り当てる / false:割り当てない</param>
        public static void SetConsoleEnabled( bool enabled)
        {
            API.LNConfig_SetConsoleEnabled( enabled);
        
        }
        
        /// <summary>
        /// ユーザー定義のウィンドウハンドルを設定します。(初期値:NULL)
        /// </summary>
        /// <param name="windowHandle">ユーザー定義のウィンドウハンドル</param>
        public static void SetUserWindowHandle( IntPtr windowHandle)
        {
            API.LNConfig_SetUserWindowHandle( windowHandle);
        
        }
        
        /// <summary>
        /// サウンドオブジェクトのキャッシュサイズの設定
        /// </summary>
        /// <param name="count">キャッシュできるサウンドオブジェクトの最大数 (初期値:32)</param>
        /// <param name="memorySize">サウンドオブジェクトのキャッシュが使用できる最大メモリサイズ (初期値:0)</param>
        /// <remarks>
        /// count が 0 の場合、キャッシュを使用しません。
        /// 					memorySize が 0 の場合、メモリ使用量に制限を設けません。
        /// </remarks>
        public static void SetSoundCacheSize( int count,  int memorySize)
        {
            API.LNConfig_SetSoundCacheSize( count,  memorySize);
        
        }
        
        /// <summary>
        /// DirectMusic の初期化方法を設定します。(初期値:DirectMusicMode.NotUse)
        /// </summary>
        /// <param name="mode">DirectMusic の初期化方法</param>
        /// <remarks>
        /// DirectMusic の初期化には比較的時間がかかります。
        /// 					これを回避するために初期化専用のスレッドで初期化を行うことが出来ます。
        /// </remarks>
        public static void SetDirectMusicInitializeMode( DirectMusicMode mode)
        {
            API.LNConfig_SetDirectMusicInitializeMode( mode);
        
        }
        
        /// <summary>
        /// DirectMusic のリバーブエフェクトの強さを設定します。(初期値:70)
        /// </summary>
        /// <param name="level">リバーブの強さ (0 ～ 100)</param>
        public static void SetDirectMusicReverbLevel( int level)
        {
            API.LNConfig_SetDirectMusicReverbLevel( level);
        
        }
        
    
    };
    
    /// <summary>
    /// LightNote の初期化や更新等、包括的な処理を行うクラスです。
    /// </summary>
    public partial class Application
    {
    
    
        /// <summary>
        /// アプリケーションを初期化します。
        /// </summary>
        /// <remarks>
        /// ライブラリのすべての機能を使用できるように初期化を行います。
        /// </remarks>
        public static void Initialize()
        {
            InternalManager.Initialize();
            var result = API.LNApplication_Initialize();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// アプリケーションを初期化します。音声機能のみを使用する場合に呼び出します。
        /// </summary>
        public static void InitializeAudio()
        {
            InternalManager.Initialize();
            var result = API.LNApplication_InitializeAudio();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// フレームを更新します。
        /// </summary>
        /// <remarks>
        /// LightNote の状態を更新し、時間を1フレーム分進めます。
        /// 					この関数は必ず1フレームに1度だけ呼び出す必要があります。
        /// </remarks>
        public static void Update()
        {
            var result = API.LNApplication_Update();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 画面の更新タイミングをリセットします。
        /// </summary>
        /// <remarks>
        /// 時間のかかる処理の後にこのメソッドを呼ぶことで、
        /// 					極端なフレームスキップが発生しないようにすることができます。
        /// </remarks>
        public static void ResetFrameDelay()
        {
            var result = API.LNApplication_ResetFrameDelay();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// アプリケーションを終了するべきかを確認します。
        /// </summary>
        /// <remarks>
        /// ウィンドウのクローズボタンが押された場合等、
        /// 					アプリケーションを終了するべき時には false を返します。
        /// </remarks>
        public static bool IsEndRequested()
        {
            var requested = new bool();
            var result = API.LNApplication_IsEndRequested(out requested);
            if (result != Result.OK) throw new LNoteException(result);
            return requested;
        
        }
        
        /// <summary>
        /// LightNote の終了処理を行います。
        /// </summary>
        public static void Finalize()
        {
            API.LNApplication_Finalize();
            InternalManager.Terminate();
        
        }
        
    
    };
    
    /// <summary>
    /// 音声再生のユーティリティクラスです。
    /// </summary>
    public partial class Audio
    {
    
    
        /// <summary>
        /// BGM を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        public static void PlayBGM( string filePath,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_PlayBGM( filePath,  volume,  pitch,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声ファイルデータを使用して BGM を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        public static void PlayBGMMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_PlayBGMMem( data,  dataSize,  volume,  pitch,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// BGM の演奏を停止します。
        /// </summary>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        public static void StopBGM( double fadeTime = 0.0)
        {
            var result = API.LNAudio_StopBGM( fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// BGS を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        public static void PlayBGS( string filePath,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_PlayBGS( filePath,  volume,  pitch,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声ファイルデータから BGS を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        public static void PlayBGSMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_PlayBGSMem( data,  dataSize,  volume,  pitch,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// BGS の演奏を停止します。、
        /// </summary>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        public static void StopBGS( double fadeTime = 0.0)
        {
            var result = API.LNAudio_StopBGS( fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// ME を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlayME( string filePath,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlayME( filePath,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声ファイルデータから ME を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlayMEMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlayMEMem( data,  dataSize,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// ME の演奏を停止します。
        /// </summary>
        public static void StopME()
        {
            var result = API.LNAudio_StopME();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// SE を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlaySE( string filePath,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySE( filePath,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// SE を演奏します。(3D サウンド)
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="position">3D 空間上の座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlaySE3D( string filePath,  Vector3 position,  float distance,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySE3D( filePath, ref position,  distance,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// SE を演奏します。(3D サウンド)
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="x">3D 空間上の X 座標</param>
        /// <param name="y">3D 空間上の Y 座標</param>
        /// <param name="z">3D 空間上の Z 座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlaySE3D( string filePath,  float x,  float y,  float z,  float distance,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySE3DXYZ( filePath,  x,  y,  z,  distance,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声データから SE を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlaySEMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySEMem( data,  dataSize,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声ファイルデータから SE を演奏します。 (3D サウンド)
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="position">3D 空間上の座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlaySE3DMem( byte[] data,  int dataSize,  Vector3 position,  float distance,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySE3DMem( data,  dataSize, ref position,  distance,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声ファイルデータから SE を演奏します。(3D サウンド)
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="x">3D 空間上の X 座標</param>
        /// <param name="y">3D 空間上の Y 座標</param>
        /// <param name="z">3D 空間上の Z 座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlaySE3DMem( byte[] data,  int dataSize,  float x,  float y,  float z,  float distance,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySE3DMemXYZ( data,  dataSize,  x,  y,  z,  distance,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// すべての SE の演奏を停止します。
        /// </summary>
        public static void StopSE()
        {
            var result = API.LNAudio_StopSE();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 再生中のBGMの音量を設定します。(フェードアウト中は無効)
        /// </summary>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        public static void SetBGMVolume( int volume,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_SetBGMVolume( volume,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 再生中のBGSの音量を設定します。(フェードアウト中は無効)
        /// </summary>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        public static void SetBGSVolume( int volume,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_SetBGSVolume( volume,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
    
    };
    
    /// <summary>
    /// 3D音声のリスナーに関する情報を表します。
    /// </summary>
    public partial class SoundListener
    {
    
        /// <summary>
        /// 3D音声のリスナーの位置
        /// </summary>
        public static Vector3 Position
        {
            set
            {
                var result = API.LNSoundListener_SetPosition(ref value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// 3D音声のリスナーの正面方向
        /// </summary>
        public static Vector3 Direction
        {
            set
            {
                var result = API.LNSoundListener_SetDirection(ref value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// 3D音声のリスナーの上方向(正面方向とは直交であること)
        /// </summary>
        public static Vector3 UpDirection
        {
            set
            {
                var result = API.LNSoundListener_SetUpDirection(ref value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// 3D音声のリスナーの速度
        /// </summary>
        public static Vector3 Velocity
        {
            set
            {
                var result = API.LNSoundListener_SetVelocity(ref value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
    
        /// <summary>
        /// 3D音声のリスナーの位置を設定します。
        /// </summary>
        /// <param name="x">3D 空間上の X 座標</param>
        /// <param name="y">3D 空間上の Y 座標</param>
        /// <param name="z">3D 空間上の Z 座標</param>
        public static void SetPositionXYZ( float x,  float y,  float z)
        {
            var result = API.LNSoundListener_SetPositionXYZ( x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 3D音声のリスナーの正面方向を設定します。
        /// </summary>
        /// <param name="x">向きの X 成分</param>
        /// <param name="y">向きの Y 成分</param>
        /// <param name="z">向きの Z 成分</param>
        public static void SetDirection( float x,  float y,  float z)
        {
            var result = API.LNSoundListener_SetDirectionXYZ( x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
        /// </summary>
        /// <param name="x">向きの X 成分</param>
        /// <param name="y">向きの Y 成分</param>
        /// <param name="z">向きの Z 成分</param>
        public static void SetUpDirection( float x,  float y,  float z)
        {
            var result = API.LNSoundListener_SetUpDirectionXYZ( x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 3D音声のリスナーの速度を設定します。
        /// </summary>
        /// <param name="x">速度の X 成分</param>
        /// <param name="y">速度の Y 成分</param>
        /// <param name="z">速度の Z 成分</param>
        public static void Velocity( float x,  float y,  float z)
        {
            var result = API.LNSoundListener_SetVelocityXYZ( x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
    
    };
    
    /// <summary>
    /// 音声データひとつ分を表し、再生などの操作を行うクラスです。
    /// </summary>
    public partial class Sound : ReferenceObject
    {
    
        /// <summary>
        /// サウンドのボリューム
        /// </summary>
        public int Volume
        {
            get
            {
                var outVolume = new int();
                var result = API.LNSound_GetVolume( _handle, out outVolume);
                if (result != Result.OK) throw new LNoteException(result);
                return outVolume;
            
            }
            
            set
            {
                var result = API.LNSound_SetVolume( _handle,  value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// サウンドのピッチ
        /// </summary>
        public int Pitch
        {
            get
            {
                var outPitch = new int();
                var result = API.LNSound_GetPitch( _handle, out outPitch);
                if (result != Result.OK) throw new LNoteException(result);
                return outPitch;
            
            }
            
            set
            {
                var result = API.LNSound_SetPitch( _handle,  value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// サウンドのループ再生の有無
        /// </summary>
        public bool LoopEnabled
        {
            set
            {
                var result = API.LNSound_SetLoopEnabled( _handle,  value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// サウンドのループ再生が有効であるかを確認します。
        /// </summary>
        public bool IsLoopEnabled
        {
            get
            {
                var outEnabled = new bool();
                var result = API.LNSound_IsLoopEnabled( _handle, out outEnabled);
                if (result != Result.OK) throw new LNoteException(result);
                return outEnabled;
            
            }
            
        }
        /// <summary>
        /// サウンドのループ再生の範囲
        /// </summary>
        /// <remarks>
        /// begin と length に 0 を指定すると、全体をループ領域として設定します。
        /// </remarks>
        public int LoopRange
        {
            set
            {
                var result = API.LNSound_SetLoopRange( _handle,  value,  value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// サウンドを 3D 音源として再生するか
        /// </summary>
        public bool 3DEnabled
        {
            set
            {
                var result = API.LNSound_Set3DEnabled( _handle,  value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// サウンドが 3D 音源であるかを確認します。
        /// </summary>
        public bool Is3DEnabled
        {
            get
            {
                var outEnabled = new bool();
                var result = API.LNSound_Is3DEnabled( _handle, out outEnabled);
                if (result != Result.OK) throw new LNoteException(result);
                return outEnabled;
            
            }
            
        }
        /// <summary>
        /// サウンド再生時の音声データの再生方法
        /// </summary>
        public void PlayingMode
        {
            get
            {
                var result = API.LNSound_GetPlayingMode( _handle,  outMode);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
            set
            {
                var result = API.LNSound_SetPlayingMode( _handle,  value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// サウンドの再生状態
        /// </summary>
        public SoundPlayingState PlayingState
        {
            get
            {
                var outState = new SoundPlayingState();
                var result = API.LNSound_GetPlayingState( _handle, out outState);
                if (result != Result.OK) throw new LNoteException(result);
                return outState;
            
            }
            
        }
        /// <summary>
        /// サウンドの再生したサンプル数
        /// </summary>
        public long PlayedSamples
        {
            get
            {
                var outSamples = new long();
                var result = API.LNSound_GetPlayedSamples( _handle, out outSamples);
                if (result != Result.OK) throw new LNoteException(result);
                return outSamples;
            
            }
            
        }
        /// <summary>
        /// サウンドの音声データ全体のサンプル数
        /// </summary>
        public long TotalSamples
        {
            get
            {
                var outSamples = new long();
                var result = API.LNSound_GetTotalSamples( _handle, out outSamples);
                if (result != Result.OK) throw new LNoteException(result);
                return outSamples;
            
            }
            
        }
        /// <summary>
        /// サウンドのサンプリングレート
        /// </summary>
        public int SamplingRate
        {
            get
            {
                var outRate = new int();
                var result = API.LNSound_GetSamplingRate( _handle, out outRate);
                if (result != Result.OK) throw new LNoteException(result);
                return outRate;
            
            }
            
        }
        /// <summary>
        /// サウンドの 3D 音源としての位置
        /// </summary>
        public Vector3 EmitterPosition
        {
            set
            {
                var result = API.LNSound_SetEmitterPosition( _handle, ref value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// サウンドの 3D 音源としての速度
        /// </summary>
        public Vector3 EmitterVelocity
        {
            set
            {
                var result = API.LNSound_SetEmitterVelocity( _handle, ref value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// サウンドの 3D 音源の減衰距離 (聴こえなくなる距離) 
        /// </summary>
        public float EmitterDistance
        {
            set
            {
                var result = API.LNSound_SetEmitterDistance( _handle,  value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
    
        internal Sound(_LNInternal i) : base(i) {}
        
        /// <summary>
        /// ファイルからサウンドオブジェクトを作成します。
        /// </summary>
        /// <param name="filePath">音声ファイルのパス</param>
        public  Sound( string filePath) : base(_LNInternal.InternalBlock)
        {
            IntPtr sound;
            var result = API.LNSound_Create( filePath, out sound);
            if (result != Result.OK) throw new LNoteException(result);
            InternalManager.RegisterWrapperObject(this, sound);
        
        }
        
        /// <summary>
        /// メモリ上の音声ファイルデータからサウンドオブジェクトを作成します。
        /// </summary>
        /// <param name="data">メモリ上の音声データへのポインタ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        public  Sound( byte[] data,  int dataSize) : base(_LNInternal.InternalBlock)
        {
            IntPtr sound;
            var result = API.LNSound_CreateMem( data,  dataSize, out sound);
            if (result != Result.OK) throw new LNoteException(result);
            InternalManager.RegisterWrapperObject(this, sound);
        
        }
        
        /// <summary>
        /// サウンドを再生します。
        /// </summary>
        public void Play()
        {
            var result = API.LNSound_Play( _handle);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// サウンドの再生を停止します。
        /// </summary>
        public void Stop()
        {
            var result = API.LNSound_Stop( _handle);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// サウンドの再生を一時停止します。
        /// </summary>
        public void Pause()
        {
            var result = API.LNSound_Pause( _handle);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// サウンドの再生を一時停止します。
        /// </summary>
        public void Resume()
        {
            var result = API.LNSound_Resume( _handle);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// サウンド音量のフェード操作を行います。
        /// </summary>
        /// <param name="targetVolume">変更先の音量</param>
        /// <param name="time">フェードにかける時間 (秒)</param>
        /// <param name="behavior">フェード完了後の動作の指定</param>
        public void FadeVolume( int targetVolume,  double time,  SoundFadeBehavior behavior)
        {
            var result = API.LNSound_FadeVolume( _handle,  targetVolume,  time,  behavior);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// サウンドの 3D 音源としての位置を設定します。
        /// </summary>
        /// <param name="x">3D 空間上の X 座標</param>
        /// <param name="y">3D 空間上の Y 座標</param>
        /// <param name="z">3D 空間上の Z 座標</param>
        public void SetEmitterPosition( float x,  float y,  float z)
        {
            var result = API.LNSound_SetEmitterPositionXYZ( _handle,  x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// サウンドの 3D 音源としての速度を設定します。
        /// </summary>
        /// <param name="x">速度の X 成分</param>
        /// <param name="y">速度の Y 成分</param>
        /// <param name="z">速度の Z 成分</param>
        public void SetEmitterVelocity( float x,  float y,  float z)
        {
            var result = API.LNSound_SetEmitterVelocityXYZ( _handle,  x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
    
    };
    
	

    

    internal enum _LNInternal
    {
        InternalBlock,
    }   

    /// <summary>
    /// 参照カウントを持つオブジェクトをラップする
    /// </summary>
    public class ReferenceObject : IDisposable
    {
        internal IntPtr _handle;

        internal ReferenceObject() {}
        internal ReferenceObject(_LNInternal i) {}

        /// <summary>
        /// ハンドルの取得
        /// </summary>
        public IntPtr Handle { get { return _handle; } }

        /// <summary>
        /// デストラクタ
        /// </summary>
        ~ReferenceObject() { Dispose(); }

        internal virtual void SetHandle(IntPtr handle)
        {
            _handle = handle;
        }       

        /// <summary>
        /// Dispose
        /// </summary>
        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                if (InternalManager.SystemInitialized)
                {
                    InternalManager.UnregisterWrapperObject(this);
                    API.LNObject_Release(_handle);
                }
                _handle = IntPtr.Zero;
            }
            GC.SuppressFinalize(this);
        }
    }

    internal class TypeInfo
    {
        public delegate ReferenceObject ReferenceObjectFactory(IntPtr handle);

        public ReferenceObjectFactory Factory;

        private static List<TypeInfo> _typeInfos = new List<TypeInfo>();

        public static void Register()
        {

var _Sound = new TypeInfo(){ Factory = (handle) =>
    {
        var obj = new Sound(_LNInternal.InternalBlock);
        obj.SetHandle(handle);
        return obj;
    }
};
_typeInfos.Add(_Sound);
LNSound_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

        }

        public static TypeInfo GetTypeInfoByHandle(IntPtr handle)
        {
            int index = (int)LNObject_GetTypeUserData(handle);
            return _typeInfos[index];
        }

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern IntPtr LNObject_GetTypeUserData(IntPtr handle);
[DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
private static extern void LNSound_SetBindingTypeInfo(IntPtr data);


    }

    /// <summary>
    /// オブジェクトのコレクション
    /// </summary>
    public class ObjectList<T> : ReferenceObject
        where T : ReferenceObject
    {
        private List<ReferenceObject> _list;
        
        internal ObjectList(_LNInternal i) {}

        internal override void SetHandle(IntPtr handle)
        {
            _list = new List<ReferenceObject>();
            _handle = handle;
            int count = Count;
            for (int i = 0; i < count; i++)
            {
                IntPtr item;
                API.LNObjectList_GetAt(_handle, i, out item);
                var t = TypeInfo.GetTypeInfoByHandle(item).Factory(item);
                t.SetHandle(item);
                _list.Add(t);
            }
        }
        
        public int Count
        {
            get
            {
                int count;
                API.LNObjectList_GetCount(_handle, out count);
                return count;
            }
        }
        
        public T this[int index]
        {
            get
            {
                SyncItems();
                return (T)_list[index];
            }
            set
            {
                SyncItems();
                _list[index] = value;
                API.LNObjectList_SetAt(_handle, index, value.Handle);
            }
        }
        
        public void Add(T item)
        {
            SyncItems();
            _list.Add(item);
            API.LNObjectList_Add(_handle, item.Handle);
        }
        
        public void Clear()
        {
            _list.Clear();
            API.LNObjectList_Clear(_handle);
        }
        
        public void Insert(int index, T item)
        {
            SyncItems();
            _list.Insert(index, item);
            API.LNObjectList_Insert(_handle, index, item.Handle);
        }
        
        public void Remove(T item)
        {
            SyncItems();
            _list.Remove(item);
            API.LNObjectList_Remove(_handle, item.Handle);
        }
        
        public void RemoveAll(T item)
        {
            SyncItems();
            _list.RemoveAll((i) => i == item);
            API.LNObjectList_RemoveAll(_handle, item.Handle);
        }
        
        public void RemoveAt(int index)
        {
            SyncItems();
            _list.RemoveAt(index);
            API.LNObjectList_RemoveAt(_handle, index);
        }

        private void SyncItems() 
        {
            int count = Count;
            if (_list.Count < count)
            {
                // 足りない分を詰める
                for (int i = 0; i < count - _list.Count; ++i)
		        {
                    _list.Add(null);
                }

                // リスト内容を同期する
                for (int i = 0; i < count; ++i)
		        {
                    IntPtr item;
                    API.LNObjectList_GetAt(_handle, i, out item);
			        if (_list[i] == null || _list[i].Handle != item)
			        {
                        var t = TypeInfo.GetTypeInfoByHandle(item).Factory(item);
                        t.SetHandle(item);
                        _list.Add(t);
			        }
		        }
            }
        }
    }

	internal class UserData
    {
        public WeakReference     RefObject;
    }

    internal static class InternalManager
    {
        private const int InitialListSize = 1024;
        
        //private static  List<ReferenceObject> _objectList;
        private static  Stack<int> _userDataListIndexStack;
        private static  List<UserData> _userDataList;           // LNObject_SetUserData で登録するのはこのインデックス。0 は無効値 (ユーザーデータ未割り当て)

		
        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
		private static extern void LNObject_SetUserData(IntPtr handle, IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
		private static extern IntPtr LNObject_GetUserData(IntPtr handle);

		// init ～ terminate
        public static bool SystemInitialized = false;

		// 初期化
		public static void Initialize()
		{
			if (!SystemInitialized)
			{
				_userDataListIndexStack = new Stack<int>(InitialListSize);
				_userDataList = new List<UserData>(InitialListSize); 

				// 管理リストの空き番号を詰める
				for (int i = 0; i < InitialListSize; i++)
                {
					_userDataListIndexStack.Push(i);
                    _userDataList.Add(null);
                }
					
				// 型情報の登録
				TypeInfo.Register();

				SystemInitialized = true;
			}
		}

		// 終了処理
		public static void Terminate()
		{
			SystemInitialized = false;
			GC.Collect();
		}

        // ハンドルからラップオブジェクトを返す
        public static T GetWrapperObject<T>(IntPtr handle) where T : ReferenceObject
        {
            int index = (int)LNObject_GetUserData(handle);
            if (index == 0) // 新しく登録する
                return CreateWrapperObject<T>(handle);
            else            // 登録済みのオブジェクトを返す
                return (T)_userDataList[index].RefObject.Target;
        }

        // 新しいハンドルを登録し、ラップオブジェクトを生成する
        public static T CreateWrapperObject<T>(IntPtr handle) where T : ReferenceObject
        {
            var obj = TypeInfo.GetTypeInfoByHandle(handle).Factory(handle);
            RegisterWrapperObject(obj, handle);
            return (T)obj;
        }

        // handle を refObj にセットし、refObj を管理リストに追加する
        //  (ReferenceObject のコンストラクタからも呼ばれる)
        public static void RegisterWrapperObject(ReferenceObject refObj, IntPtr handle)
        {
            // 管理リストが一杯の時は、倍のサイズに拡張する
            if (_userDataListIndexStack.Count == 0)
            {
                int growCount = _userDataList.Count;
                _userDataList.Capacity = growCount + growCount;
                for (int i = 0; i < growCount; i++)
                {
                    _userDataList.Add(null);
                    _userDataListIndexStack.Push(growCount + i);
                }
            }

            // 空きインデックスに new する
            //      new T ではなくちゃんとファクトリから作る。 new T ではサブクラスを考慮できない。
            //      コレクションの取得で Sprite を作りたいのに SceneNode を作ってしまうかもしれない。
            int index = _userDataListIndexStack.Pop();
            refObj.SetHandle(handle);
            _userDataList[index].RefObject = new WeakReference(refObj);

            // 管理配列のインデックスをユーザーデータとして関連付ける
            LNObject_SetUserData(handle, (IntPtr)index);
        }

        // RefObject の Dispose から呼ばれ、管理リストのインデックスを解放する
        //      (ユーザーデータを取得するため、LNObject_Release より前に呼ぶこと)
        public static void UnregisterWrapperObject(ReferenceObject refObj)
        {
            int index = (int)LNObject_GetUserData(refObj.Handle);
            _userDataList[index] = null;
            _userDataListIndexStack.Push(index);
        }
    }

}