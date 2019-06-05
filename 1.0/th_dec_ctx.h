typedef struct oc_theora_state oc_theora_state;

typedef int         oc_sb_map[4][4];
typedef int         oc_mb_map[3][4];
typedef signed char oc_mv[2];

typedef struct {
    int           mode;
    int           x;
    int           y;
    oc_mb_map     map;
} oc_mb; 

typedef struct {
    unsigned  coded_fully:1;
    unsigned  coded_partially:1;
    unsigned  quad_valid:4;
    oc_sb_map map;
} oc_sb;

typedef struct {
    ogg_int64_t mask;
    int         npixels;
} oc_border_info;

typedef struct {
    unsigned        coded:1;
    unsigned        invalid:1;
    unsigned        qi:6;
    signed int      mbmode:8;
    signed int      dc:16;
    unsigned char  *buffer[4];
    oc_border_info *border;
    oc_mv           mv;
} oc_fragment;

typedef struct {
    int nhfrags;
    int nvfrags;
    int froffset;
    int nfrags;
    int nhsbs;
    int nvsbs;
    int sboffset;
    int nsbs;
} oc_fragment_plane;

typedef struct {
 	void (*frag_recon_intra)(unsigned char *_dst,int _dst_ystride,const ogg_int16_t *_residue);
  	void (*frag_recon_inter)(unsigned char *_dst,int _dst_ystride,const unsigned char *_src,int _src_ystride,const ogg_int16_t *_residue);
  	void (*frag_recon_inter2)(unsigned char *_dst,int _dst_ystride,const unsigned char *_src1,int _src1_ystride,const unsigned char *_src2,int _src2_ystride,const ogg_int16_t *_residue);
    void (*state_frag_copy)(const oc_theora_state *_state,const int *_fragis,int _nfragis,int _dst_frame,int _src_frame,int _pli);
  	void (*state_frag_recon)(oc_theora_state *_state,oc_fragment *_frag,int _pli,ogg_int16_t _dct_coeffs[128],int _last_zzi,int _ncoefs,
    ogg_uint16_t _dc_iquant,const ogg_uint16_t _ac_iquant[64]);
 	void (*restore_fpu)(void);
  	void (*state_loop_filter_frag_rows)(oc_theora_state *_state,int *_bv,int _refi,int _pli,int _fragy0,int _fragy_end);  
} oc_base_opt_vtable;
 
typedef ogg_uint16_t   oc_quant_table[64];
typedef oc_quant_table oc_quant_tables[64];

struct oc_theora_state {
    th_info             info;
    oc_base_opt_vtable  opt_vtable;
    ogg_uint32_t        cpu_flags;
    oc_fragment_plane   fplanes[3];
    int                 nfrags;
    oc_fragment        *frags;
    int                 nsbs;
    oc_sb              *sbs;
    int                 nhmbs;
    int                 nvmbs;
    int                 nmbs;
    oc_mb              *mbs;
    int                *coded_fragis;
    int                 ncoded_fragis[3];
    int                *uncoded_fragis;
    int                 nuncoded_fragis[3];
    int                *coded_mbis;
    int                 ncoded_mbis;
    th_ycbcr_buffer     input;
    int                 nborders;
    oc_border_info      borders[16];
    int                 ref_frame_idx[3];
    th_ycbcr_buffer     ref_frame_bufs[3];
    unsigned char      *ref_frame_data;
    ogg_int64_t         keyframe_num;
    ogg_int64_t         curframe_num;
    ogg_int64_t         granpos;
    int                 frame_type;
    int                 qis[3];
    int                 nqis;
    oc_quant_table     *dequant_tables[2][3];
    oc_quant_tables     dequant_table_data[2][3];
    unsigned char       loop_filter_limits[64];
};

struct oc_huff_node {
    unsigned char  nbits;
    unsigned char  token;
    unsigned char  depth;
    oc_huff_node  *nodes[1];
};

struct th_dec_ctx{
    oc_theora_state      state;
    int                  packet_state;
    oggpack_buffer       opb;
    oc_huff_node        *huff_tables[TH_NHUFFMAN_TABLES];
    int                  ti0[3][64];
    int                  ebi0[3][64];
    int                  eob_runs[3][64];
    unsigned char      **dct_tokens;
    ogg_uint16_t       **extra_bits;
    int                  pp_level;
    int                  pp_dc_scale[64];
    int                  pp_sharp_mod[64];
    unsigned char       *dc_qis;
    int                 *variances;
    unsigned char       *pp_frame_data;
    int                  pp_frame_has_chroma;
    th_ycbcr_buffer      pp_frame_buf;
    th_stripe_callback   stripe_cb;
};