SECTIONS
{
        .mtk_bsp2 (NOLOAD) :
        {
                *(.mtk_exctbl)
                __mtk3_SYSMEM_START = .;
        }>RAM
}
