typedef struct oc_theora_state oc_theora_state;

struct oc_base_opt_data {
    const unsigned char *dct_fzig_zag;
};
struct oc_base_opt_vtable {
    void (*frag_copy)(unsigned char *_dst,const unsigned char *_src,int _ystride);
    void (*frag_recon_intra)(unsigned char *_dst,int _ystride,const ogg_int16_t _residue[64]);
    void (*frag_recon_inter)(unsigned char *_dst,const unsigned char *_src,int _ystride,const ogg_int16_t _residue[64]);
    void (*frag_recon_inter2)(unsigned char *_dst,const unsigned char *_src1,const unsigned char *_src2,int _ystride,const ogg_int16_t _residue[64]);
    void (*idct8x8)(ogg_int16_t _y[64],int _last_zzi);
    void (*state_frag_recon)(const oc_theora_state *_state,ptrdiff_t _fragi,int _pli,ogg_int16_t _dct_coeffs[64],int _last_zzi,ogg_uint16_t _dc_quant);
    void (*state_frag_copy_list)(const oc_theora_state *_state,const ptrdiff_t *_fragis,ptrdiff_t _nfragis,int _dst_frame,int _src_frame,int _pli);
    void (*state_loop_filter_frag_rows)(const oc_theora_state *_state,int _bv[256],int _refi,int _pli,int _fragy0,int _fragy_end);
    void (*restore_fpu)(void);
};

struct oc_fragment_plane {
    int       nhfrags;
    int       nvfrags;
    ptrdiff_t froffset;
    ptrdiff_t nfrags;
    unsigned  nhsbs;
    unsigned  nvsbs;
    unsigned  sboffset;
    unsigned  nsbs;
};

struct oc_fragment {
    unsigned   coded:1;
    unsigned   invalid:1;
    unsigned   qii:6;
    unsigned   mb_mode:3;
    signed int borderi:5;
    signed int dc:16;
};

typedef ptrdiff_t       oc_sb_map_quad[4];
typedef oc_sb_map_quad  oc_sb_map[4];
typedef ptrdiff_t       oc_mb_map_plane[4];
typedef oc_mb_map_plane oc_mb_map[3];
typedef signed char     oc_mv[2];

struct oc_sb_flags {
    unsigned char coded_fully:1;
    unsigned char coded_partially:1;
    unsigned char quad_valid:4;
};

struct oc_border_info {
    ogg_int64_t mask;
    int npixels;
};

#if !defined(OC_ALIGN16)
    #define OC_ALIGN16(expr) expr
#endif

typedef ogg_uint16_t oc_quant_table[64];

struct oc_theora_state {
    th_info             info;
    oc_base_opt_vtable  opt_vtable;
    oc_base_opt_data    opt_data;
    ogg_uint32_t        cpu_flags;
    oc_fragment_plane   fplanes[3];
    oc_fragment        *frags;
    ptrdiff_t          *frag_buf_offs;
    oc_mv              *frag_mvs;
    ptrdiff_t           nfrags;
    oc_sb_map          *sb_maps;
    oc_sb_flags        *sb_flags;
    unsigned            nsbs;
    oc_mb_map          *mb_maps;
    signed char        *mb_modes;
    unsigned            nhmbs;
    unsigned            nvmbs;
    size_t              nmbs;
    ptrdiff_t          *coded_fragis;
    ptrdiff_t           ncoded_fragis[3];
    ptrdiff_t           ntotal_coded_fragis;
    int                 ref_frame_idx[4];
    th_ycbcr_buffer     ref_frame_bufs[4];
    unsigned char      *ref_frame_data[4];
    int                 ref_ystride[3];
    int                 nborders;
    oc_border_info      borders[16];
    ogg_int64_t         keyframe_num;
    ogg_int64_t         curframe_num;
    ogg_int64_t         granpos;
    unsigned char       frame_type;
    unsigned char       granpos_bias;
    unsigned char       nqis;
    unsigned char       qis[3];
    ogg_uint16_t       *dequant_tables[64][3][2];
    OC_ALIGN16(oc_quant_table dequant_table_data[64][3][2]);
    unsigned char       loop_filter_limits[64];
};

typedef unsigned long oc_pb_window;

struct oc_pack_buf {
    oc_pb_window         window;
    const unsigned char *ptr;
    const unsigned char *stop;
    int                  bits;
    int                  eof;
};

struct oc_huff_node {
    unsigned char  nbits;
    unsigned char  token;
    unsigned char  depth;
    oc_huff_node  *nodes[2];
};

struct th_dec_ctx {
    oc_theora_state      state;
    int                  packet_state;
    oc_pack_buf          opb;
    oc_huff_node        *huff_tables[TH_NHUFFMAN_TABLES];
    ptrdiff_t            ti0[3][64];
    ptrdiff_t            eob_runs[3][64];
    unsigned char       *dct_tokens;
    unsigned char       *extra_bits;
    int                  dct_tokens_count;
    int                  pp_level;
    int                  pp_dc_scale[64];
    int                  pp_sharp_mod[64];
    unsigned char       *dc_qis;
    int                 *variances;
    unsigned char       *pp_frame_data;
    int                  pp_frame_state;
    th_ycbcr_buffer      pp_frame_buf;
    th_stripe_callback   stripe_cb;
};