#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Lane Curve Demo - Simplified Version
既存の.soファイルを確認し、プロジェクト構成を説明するスクリプト
"""

import sys
from pathlib import Path


def main():
    print("=" * 60)
    print("Lane Curve Library - Project Structure Verification")
    print("=" * 60)
    print()
    
    # プロジェクトルートを取得
    script_dir = Path(__file__).parent.parent
    
    # liblanecurve_pythonプロジェクトの確認
    print("1. liblanecurve_python (共有ライブラリプロジェクト)")
    print("-" * 60)
    lib_project = script_dir / "liblanecurve_python"
    if lib_project.exists():
        print(f"✓ プロジェクトディレクトリ: {lib_project}")
        
        # 主要なファイルを確認
        files_to_check = [
            "CMakeLists.txt",
            "README.md",
            "include/interpolators/kalman_interpolator.h",
            "include/interpolators/interpolator.h",
            "include/utils/image_utils.h",
            "include/python_wrapper.h",
            "src/interpolators/kalman_interpolator.cpp",
            "src/utils/line_segment.cpp",
            "src/utils/image_utils.cpp",
            "src/wrappers/interpolators_wrapper.cpp",
            "src/wrappers/utils_wrapper.cpp",
        ]
        
        for file_path in files_to_check:
            full_path = lib_project / file_path
            status = "✓" if full_path.exists() else "✗"
            print(f"  {status} {file_path}")
    else:
        print(f"✗ プロジェクトが見つかりません: {lib_project}")
    
    print()
    
    # lanecurve_python_demoプロジェクトの確認
    print("2. lanecurve_python_demo (Pythonデモプロジェクト)")
    print("-" * 60)
    demo_project = script_dir / "lanecurve_python_demo"
    if demo_project.exists():
        print(f"✓ プロジェクトディレクトリ: {demo_project}")
        
        # 主要なファイルを確認
        files_to_check = [
            "demo_main.py",
            "README.md",
            "lib/libinterpolators.so",
            "lib/libutils.so",
        ]
        
        for file_path in files_to_check:
            full_path = demo_project / file_path
            status = "✓" if full_path.exists() else "✗"
            print(f"  {status} {file_path}")
            
            # .soファイルの詳細情報
            if file_path.endswith('.so') and full_path.exists():
                size = full_path.stat().st_size
                print(f"      サイズ: {size:,} bytes")
    else:
        print(f"✗ プロジェクトが見つかりません: {demo_project}")
    
    print()
    
    # ビルド手順の説明
    print("3. ビルドとセットアップ手順")
    print("-" * 60)
    print("次のコマンドを実行してプロジェクトをビルドします:")
    print()
    print("  cd liblanecurve_python")
    print("  mkdir build && cd build")
    print("  cmake -DCMAKE_BUILD_TYPE=Release ..")
    print("  make")
    print("  cd ../..")
    print()
    print("または、自動ビルドスクリプトを使用:")
    print()
    print("  ./build_all.sh")
    print()
    
    # 実行手順の説明
    print("4. デモの実行手順")
    print("-" * 60)
    print("ビルド後、以下のコマンドでデモを実行できます:")
    print()
    print("  cd lanecurve_python_demo")
    print("  python3 demo_main.py")
    print()
    print("出力ファイルは output/ ディレクトリに生成されます:")
    print("  - equation.txt (推定された曲線の方程式)")
    print("  - lane_curve.ppm (可視化画像)")
    print()
    
    # プロジェクト構造の説明
    print("5. プロジェクト構造")
    print("-" * 60)
    print("""
    project_root/
    ├── README.md                    # プロジェクト全体の説明
    ├── build_all.sh                 # 自動ビルドスクリプト
    │
    ├── liblanecurve_python/         # C++ライブラリプロジェクト
    │   ├── CMakeLists.txt          # ビルド設定
    │   ├── README.md               # ライブラリの説明
    │   ├── include/                # ヘッダーファイル
    │   │   ├── interpolators/
    │   │   ├── utils/
    │   │   └── python_wrapper.h
    │   ├── src/                    # ソースファイル
    │   │   ├── interpolators/
    │   │   ├── utils/
    │   │   └── wrappers/
    │   └── build/                  # ビルド成果物
    │       ├── libinterpolators.so
    │       └── libutils.so
    │
    └── lanecurve_python_demo/      # Pythonデモプロジェクト
        ├── demo_main.py            # デモプログラム
        ├── README.md               # デモの説明
        ├── lib/                    # 共有ライブラリ
        │   ├── libinterpolators.so
        │   └── libutils.so
        └── output/                 # 実行結果
            ├── equation.txt
            └── lane_curve.ppm
    """)
    
    print("=" * 60)
    print("プロジェクト構成の確認が完了しました")
    print("=" * 60)
    
    return 0


if __name__ == "__main__":
    sys.exit(main())
