import os
import sys
from pathlib import Path

def is_text_file(file_path):
    """
    判断文件是否为纯文本文件
    """
    try:
        # 常见的文本文件扩展名
        text_extensions = {
            '.txt', '.py', '.js', '.java', '.c', '.cpp', '.h', '.hpp',
            '.html', '.css', '.xml', '.json', '.csv', '.md', '.rst',
            '.php', '.rb', '.go', '.rs', '.swift', '.kt', '.sql',
            '.sh', '.bat', '.ps1', '.yml', '.yaml', '.ini', '.cfg',
            '.conf', '.properties', '.log'
        }
        
        # 通过扩展名判断
        if Path(file_path).suffix.lower() in text_extensions:
            return True
        
        # 对于没有扩展名或未知扩展名的文件，尝试读取内容判断
        with open(file_path, 'rb') as f:
            chunk = f.read(1024)  # 读取前1024字节
            if b'\0' in chunk:  # 如果包含空字符，可能是二进制文件
                return False
            # 检查是否包含大量非文本字符
            text_chars = bytearray({7,8,9,10,12,13,27} | set(range(0x20, 0x100)) - {0x7f})
            return not bool(chunk.translate(None, text_chars))
            
    except Exception:
        return False

def count_lines_in_file(file_path):
    """
    统计单个文件的行数
    """
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            return sum(1 for _ in f)
    except Exception as e:
        print(f"无法读取文件 {file_path}: {e}")
        return 0

def count_lines_in_directory(directory_path):
    """
    统计目录中所有纯文本文件的行数
    """
    total_lines = 0
    file_count = 0
    
    directory_path = Path(directory_path)
    
    if not directory_path.exists():
        print(f"错误：目录 '{directory_path}' 不存在")
        return 0, 0
    
    print(f"正在统计目录: {directory_path.absolute()}")
    print("-" * 50)
    
    # 遍历目录中的所有文件
    for file_path in directory_path.rglob('*'):
        if file_path.is_file():
            # 跳过隐藏文件和系统文件
            if file_path.name.startswith('.') or file_path.name.startswith('__'):
                continue
                
            if is_text_file(file_path):
                lines = count_lines_in_file(file_path)
                total_lines += lines
                file_count += 1
                print(f"{file_path.relative_to(directory_path)}: {lines} 行")
    
    return total_lines, file_count

def main():
    """
    主函数
    """
    if len(sys.argv) != 2:
        print("用法: python line_counter.py <目录路径>")
        print("示例: python line_counter.py ./src")
        sys.exit(1)
    
    directory = sys.argv[1]
    
    print("代码行数统计工具")
    print("=" * 50)
    
    total_lines, file_count = count_lines_in_directory(directory)
    
    print("-" * 50)
    print(f"统计完成！")
    print(f"文件数量: {file_count}")
    print(f"总行数: {total_lines}")

if __name__ == "__main__":
    main()
