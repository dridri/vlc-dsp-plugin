#define DOMAIN  "vlc-dsp"
#define _(str)  dgettext(DOMAIN, str)
#define N_(str) (str)

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_charset.h>
#include <vlc_configuration.h>
#include <vlc_variables.h>

#include <vlc_aout.h>
#include <vlc_filter.h>


static int Open( vlc_object_t* p_this );
static void Close( vlc_object_t* p_this );
static block_t * DoWork( filter_t * p_filter, block_t * p_in_buf );
static void ( *dspCallback ) ( float* samples, int nb_channels, int nb_samples ) = 0;


/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
vlc_module_begin ()
	//set_text_domain(DOMAIN)
	set_description( N_("DSP Filter") )
	set_shortname( N_("dsp" ) )
	set_capability( "audio filter", 0 )
	set_category( CAT_AUDIO )
	set_subcategory( SUBCAT_AUDIO_AFILTER )
	set_callbacks( Open, Close )
	add_string("dsp-callback", "0", "dsp-callback", "dsp-callback", false)
vlc_module_end ()


static int Open( vlc_object_t* p_this )
{
	filter_t* p_filter = (filter_t*)p_this;

//	p_filter->fmt_in.audio.i_format = VLC_CODEC_S16N;
	p_filter->fmt_in.audio.i_format = VLC_CODEC_FL32;
	p_filter->fmt_out.audio = p_filter->fmt_in.audio;
	p_filter->pf_audio_filter = DoWork;

    char* str_cb = var_InheritString(p_this, "dsp-callback");
    if ( str_cb == NULL)
    {
        msg_Err(p_filter, "No callback specified !\n");
        getchar();
		return -1;
    }
    sscanf(str_cb, "%p", &dspCallback);
    printf("DSP Callback : %p\n", dspCallback);

	return VLC_SUCCESS;
}

static void Close( vlc_object_t* p_this )
{
	filter_t* p_filter = (filter_t*)p_this;
	( void ) p_filter;
}

static block_t * DoWork( filter_t * p_filter, block_t * p_in_buf )
{
	uint32_t nb_channels = aout_FormatNbChannels( &p_filter->fmt_in.audio );
	uint32_t nb_samples = p_in_buf->i_nb_samples;

	if ( dspCallback != 0 )
	{
		dspCallback( ( float* )p_in_buf->p_buffer, nb_channels, nb_samples );
	}
	return p_in_buf;
}
