# chapter 4
ファイルシステムを探求するーpwdを書く

実際のディレクトリは、ファイル名とinode番号のペアがリストになったものを格納するファイルである。
それ以上の何ものでもない

1. pwd は何をするのか

2. pwd はどんな仕組みなのか

- "."のiノード番号を記録し、それをnと呼ぶ（statを使う）
- chdir .. をする (chdirを使う)
- nというiノードを持つリンクの名前を探す ( opendir, readdir, closedir を使う )
　以上をツリーのトップに達するまで繰り返す。


次は何か
　ファイルはデータの供給源の１つにすぎない。プログラムは、端末、カメラ、スキャナなど
のデバイスから送られてきたデータも処理する。Unixプログラムは、どのようにしてデバイス
からデータを読み出し、デバイスにデータを送りつけるのだろうか。

