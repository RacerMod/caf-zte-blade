/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "msm_fb.h"
#include "mipi_dsi.h"
#include "mipi_renesas.h"

static struct msm_panel_info pinfo;

static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* DSI_BIT_CLK at 400MHz, 1 lane, RGB888 */
	/* regulator */
	{0x03, 0x01, 0x01, 0x00},
	/* timing   */
	{0xaa, 0x3b, 0x1b, 0x00, 0x52, 0x58, 0x20, 0x3f,
	0x2e, 0x03, 0x04},
	/* phy ctrl */
	{0x7f, 0x00, 0x00, 0x00},
	/* strength */
	{0xee, 0x00, 0x86, 0x00},
	/* pll control */
	{0x40, 0xc7, 0xb0, 0xda, 0x00, 0x50, 0x48, 0x63,
#if defined(RENESAS_FWVGA_TWO_LANE)
	0x30, 0x07, 0x03,
#else
	/* default set to 1 lane */
	0x30, 0x07, 0x07,
#endif
	0x05, 0x14, 0x03, 0x0, 0x0, 0x54, 0x06, 0x10, 0x04, 0x0},
};

static int __init mipi_video_renesas_fwvga_pt_init(void)
{
	int ret;

#ifdef CONFIG_FB_MSM_MIPI_PANEL_DETECT
	if (msm_fb_detect_client("mipi_video_renesas_fwvga"))
		return 0;
#endif

	pinfo.xres = 480;
	pinfo.yres = 864;
	pinfo.type = MIPI_VIDEO_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;
#if defined(RENESAS_FWVGA_TWO_LANE)
	pinfo.lcdc.h_back_porch = 400;
#else
	pinfo.lcdc.h_back_porch = 50;
#endif
	pinfo.lcdc.h_front_porch = 50;

#if defined(RENESAS_FWVGA_TWO_LANE)
	pinfo.lcdc.h_pulse_width = 5;
#else
	pinfo.lcdc.h_pulse_width = 20;
#endif

#if defined(RENESAS_FWVGA_TWO_LANE)
	pinfo.lcdc.v_back_porch = 75;
	pinfo.lcdc.v_front_porch = 5;
	pinfo.lcdc.v_pulse_width = 1;
#else
	pinfo.lcdc.v_back_porch = 10;
	pinfo.lcdc.v_front_porch = 10;
	pinfo.lcdc.v_pulse_width = 5;
#endif
	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff;	/* blue */
	pinfo.lcdc.hsync_skew = 0;
	pinfo.bl_max = 15;
	pinfo.bl_min = 1;
	pinfo.fb_num = 2;

	pinfo.mipi.mode = DSI_VIDEO_MODE;
	pinfo.mipi.pulse_mode_hsa_he = TRUE;
	pinfo.mipi.hfp_power_stop = TRUE;
	pinfo.mipi.hbp_power_stop = TRUE;
	pinfo.mipi.hsa_power_stop = TRUE;
	pinfo.mipi.eof_bllp_power_stop = TRUE;
	pinfo.mipi.bllp_power_stop = TRUE;
	pinfo.mipi.traffic_mode = DSI_NON_BURST_SYNCH_PULSE;
	pinfo.mipi.dst_format = DSI_VIDEO_DST_FORMAT_RGB888;
	pinfo.mipi.vc = 0;
	pinfo.mipi.rgb_swap = DSI_RGB_SWAP_BGR;
	pinfo.mipi.data_lane0 = TRUE;
#if defined(RENESAS_FWVGA_TWO_LANE)
	pinfo.mipi.data_lane1 = TRUE;
#else
	pinfo.mipi.data_lane1 = FALSE;
#endif
	pinfo.mipi.t_clk_post = 0x03;
	pinfo.mipi.t_clk_pre = 0x24;
	pinfo.mipi.stream = 0; /* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.frame_rate = 60;
	pinfo.mipi.dsi_phy_db = &dsi_video_mode_phy_db;

	ret = mipi_renesas_device_register(&pinfo, MIPI_DSI_PRIM,
						MIPI_DSI_PANEL_FWVGA_PT);
	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	return ret;
}

module_init(mipi_video_renesas_fwvga_pt_init);