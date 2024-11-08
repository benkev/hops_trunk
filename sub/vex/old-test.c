#include <stdio.h>
#include "vex.h"
#include "mk4_vex.h"

char progname[] = "test";
int msglev = 1;

main (argc, argv)
int argc;
char **argv;
    {
    int i, j, ret;
    struct vex vs;
    struct pbs_init_struct *pbi;
printf ("started\n");

/*     ret = get_vex ("lvex_example", LVEX, "", &vs); */
/*     ret = get_vex ("evex_example", EVEX, "2611_std", &vs); */
/*     ret = get_vex ("ovex_example", OVEX, "", &vs); */
/*     printf ("nst = %d\n", vs.ovex->nst); */
/*     for (i=0; i<vs.ovex->nst; i++) */
/*         printf ("modulation = %d\n", vs.ovex->st[i].modulation);fflush(stdout); */


/*     if (ret == 0)  */
/*         { */
/*         printf ("exper_num = %d\n", vs.evex->exper_num); */
/*         printf ("tape_mode, mirror = %d, %d\n", vs.evex->tape_mode, */
/*                         vs.evex->mirror); */
/*         }  */
/*  */
    ret = get_vex ("ivex_example", IVEX, "INIT", &vs);
    if (ret == 0)
        {
        printf ("system_tempo = %g\n", vs.ivex->system_tempo);
        printf ("bocf = %d\n", vs.ivex->bocf_period);
        for (i=0; i<2; i++)
            printf ("CUCC: crate %d, server '%s', port A=%d, portB=%d\n",
                   vs.ivex->cucc_ports[i].crate_number,
                   vs.ivex->cucc_ports[i].server_envname,
                   vs.ivex->cucc_ports[i].port_A_number,
                   vs.ivex->cucc_ports[i].port_B_number);
        printf ("Counter envname, port = '%s', %d\n", vs.ivex->counter_envname,
                                vs.ivex->counter_port);
        printf ("Switch envname, port = '%s', %d\n", vs.ivex->switch_envname,
                                vs.ivex->switch_port);
        printf ("edit mask = %x\n", vs.ivex->cf_edit_mask);
        printf ("ivex timing monitor: '%s', %d, %g, %g, '%s'\n",
                vs.ivex->timing_monitor.measurement_name,
                vs.ivex->timing_monitor.switch_pos,
                vs.ivex->timing_monitor.minimum,
                vs.ivex->timing_monitor.maximum,
                vs.ivex->timing_monitor.fail_action);
        printf ("nbps = %d\n", vs.ivex->nplayback_systems);
        for (i=0; i<vs.ivex->nplayback_systems; i++)
            {
            pbi = vs.ivex->pbs_init + i;
            printf ("SU_ID, name, tracks = %d, '%s'\n", pbi->SU_ID, pbi->cHost);
            printf ("Servo interval = %f\n", pbi->tape_servo_interval);
            printf ("SUCC: server '%s', console=%d, DPU=%d\n",
                   pbi->succ_port.server_envname,
                   pbi->succ_port.console_port,
                   pbi->succ_port.DPU_port);
        printf ("pbs timing monitor: '%s', %d, %g, %g, '%s'\n",
                pbi->timing_monitor.measurement_name,
                pbi->timing_monitor.switch_pos,
                pbi->timing_monitor.minimum,
                pbi->timing_monitor.maximum,
                pbi->timing_monitor.fail_action);
            printf ("%d %d %d %d %d %d %d %d\n",
                pbi->TRM_config.mux4_2bit[0],
                pbi->TRM_config.mux4_2bit[1],
                pbi->TRM_config.mux4_2bit[2],
                pbi->TRM_config.mux4_2bit[3],
                pbi->TRM_config.mux4_2bit[4],
                pbi->TRM_config.mux4_2bit[5],
                pbi->TRM_config.mux4_2bit[6],
                pbi->TRM_config.mux4_2bit[7]);
            printf ("%d %d %d %d\n",
                pbi->TRM_config.mux4_1bit[0],
                pbi->TRM_config.mux4_1bit[1],
                pbi->TRM_config.mux4_1bit[2],
                pbi->TRM_config.mux4_1bit[3]);
            printf ("%d %d %d %d\n",
                pbi->TRM_config.mux2_2bit[0],
                pbi->TRM_config.mux2_2bit[1],
                pbi->TRM_config.mux2_2bit[2],
                pbi->TRM_config.mux2_2bit[3]);
            printf ("%d %d\n",
                pbi->TRM_config.mux2_1bit[0],
                pbi->TRM_config.mux2_1bit[1]);
            printf ("%d %d\n",
                pbi->TRM_config.mux1_2bit[0],
                pbi->TRM_config.mux1_2bit[1]);
            printf ("%d\n", pbi->TRM_config.mux1_1bit);
            printf ("pe_limit = %g\n", pbi->TRM_config.parity_error_limit);
            printf ("IFC: %d %d %d %d\n",
                pbi->TRM_config.invalid_frame_control.CRC_error,
                pbi->TRM_config.invalid_frame_control.PE_limit_exceeded,
                pbi->TRM_config.invalid_frame_control.lost_sync,
                pbi->TRM_config.invalid_frame_control.unexpected_sync);
            printf ("drive=%d\n", pbi->drive.drive_type);
            printf ("mcb address = %x:%x\n", pbi->drive.mcb_addr_start,
                                                 pbi->drive.mcb_addr_id);
            printf ("cdiam=%g\n", pbi->drive.capstan_relative_diameter);
            printf ("t_acc=%g\n", pbi->drive.tape_acceleration);
            printf ("nheads=%d\n", pbi->drive.nheads);
            printf ("vacuum = %d %d %d %d\n",
                pbi->drive.vacuum[0].inches_of_H2O,
                pbi->drive.vacuum[0].setting,
                pbi->drive.vacuum[1].inches_of_H2O,
                pbi->drive.vacuum[1].setting);
            for (j=0; j<pbi->drive.nheads; j++)
                {
                printf ("HDconn = %d %d %d\n",
                    pbi->drive.DIM_connect[j].headstack,
                    pbi->drive.DIM_connect[j].parity,
                    pbi->drive.DIM_connect[j].eqlzr);
                printf ("monitor = %d %d %d\n",
                    pbi->drive.monitor_module[j+1].track[0],
                    pbi->drive.monitor_module[j+1].track[1],
                    pbi->drive.monitor_module[j+1].equalizer);
                printf ("HSparms= %g %g %g %g %g %g %g %g\n",
                    pbi->drive.headstack[j+1].param[0],
                    pbi->drive.headstack[j+1].param[1],
                    pbi->drive.headstack[j+1].param[2],
                    pbi->drive.headstack[j+1].param[3],
                    pbi->drive.headstack[j+1].param[4],
                    pbi->drive.headstack[j+1].param[5],
                    pbi->drive.headstack[j+1].param[6],
                    pbi->drive.headstack[j+1].param[7]);
                }
            printf ("corr_seg = %d %d %d %d\n",
                pbi->su_connect.corr_segment[0],
                pbi->su_connect.corr_segment[1],
                pbi->su_connect.corr_segment[2],
                pbi->su_connect.corr_segment[3]);
            printf ("ichan_grp = %d %d %d %d\n",
                pbi->su_connect.inputbd_chan_grp[0],
                pbi->su_connect.inputbd_chan_grp[1],
                pbi->su_connect.inputbd_chan_grp[2],
                pbi->su_connect.inputbd_chan_grp[3]);
            }
        }

    printf ("Done ivex\n");fflush(stdout);
/*     ret = get_vex ("gl21a.vex", OVEX | WANT_OUTPUT, "No0040", &vs); */
/*     if (ret == 0)  */
/*         { */
/*         printf("OK\n"); */
/*         } */

    }
