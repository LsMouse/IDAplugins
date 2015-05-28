#ifndef __ELF32_Header_H_
#define __ELF32_Header_H_
#define EI_NIDENT	16
//定义ELF Header
struct str_ELF_HEADER{
	uchar		e_ident[EI_NIDENT];			// 标识
	Elf32_Half	e_type;						// object 类型
	Elf32_Half	e_machine;					// 体系结构
	Elf32_Half	e_version;					// object 文件版本
	Elf32_Word	e_entry;					// 传输控制的虚拟地址
	Elf32_Addr	e_phoff;					// PHT（ program header table）文件中的偏移量
	Elf32_Off	e_shoff;					// SHT（ section header table）文件中的偏移量
	Elf32_Word	e_flags;					// 该成员保存着相关文件的特定处理器标志。
	Elf32_Half	e_ehsize;					// 该成员保存着 ELF 头大小(以字节计数) 
	Elf32_Half	e_phentsize;				//该成员保存着在文件的PHT中一个入口的大小(以字节计数)。
	Elf32_Half	e_phnum;					//该成员保存着在程序头表中入口的个数
	Elf32_Half	e_shentsize;				//该成员保存着 section 头的大小(以字节计数)
	Elf32_Half	e_shnum;					//该成员保存着在 section header table 中的入口数目
	Elf32_Half	e_shstrndx;					//该成员保存着跟 section 名字字符表相关入口的 section 头表(section header table) 索引
}ELF_HEADER, *LP_ELF_HEADER;
//



#endif