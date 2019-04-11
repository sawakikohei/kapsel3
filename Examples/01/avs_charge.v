APPS.MultiWindowApp MultiWindowApp#1 {
   UI {
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
                  <-.<-.<-.<-.bounds.UIpanel.option,
                  <-.<-.<-.<-.Basic_Axis3D.panel.option,
                  <-.<-.<-.<-.orthoslice.OrthoSliceUI.UIpanel.option,
                  <-.<-.<-.<-.set_minmax.UIpanel.option,
                  <-.<-.<-.<-.isoline.UIpanel.option,
                  <-.<-.<-.<-.orthoslice#1.OrthoSliceUI.UIpanel.option,
                  <-.<-.<-.<-.downsize.panel.option,
                  <-.<-.<-.<-.combine_vect.CombVectUI.UIpanel.option,
                  <-.<-.<-.<-.glyph.UIpanel.option,
                  <-.<-.<-.<-.orthoslice#2.OrthoSliceUI.UIpanel.option,
                  <-.<-.<-.<-.set_minmax#1.UIpanel.option,
                  <-.<-.<-.<-.isovolume.UIpanel.option,
                  <-.<-.<-.<-.LegendVert.UImod_panel.option,
                  <-.<-.<-.<-.set_minmax#2.UIpanel.option,
                  <-.<-.<-.<-.isosurface.UIpanel.option,
                  <-.<-.<-.<-.LegendHoriz.UImod_panel.option,
                  <-.<-.<-.<-.OutputImage.output_field.write_image_ui.panel.option,
                  <-.<-.<-.<-.OutputVPS.vpsUI.vpsModPanel.option,
                  <-.<-.<-.<-.image_capture.UImod_panel.option
               };
               selectedItem = 0;
            };
            mod_panel {
               x = 0;
               y = 0;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=341.,NEy=418.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.bounds.out_obj,<-.<-.<-.Basic_Axis3D.out_obj,
               <-.<-.<-.glyph.out_obj,<-.<-.<-.isosurface.out_obj,
               <-.<-.<-.LegendVert.GroupObject.obj,<-.<-.<-.isovolume.out_obj
            };
            Xform {
               ocenter = {29.75,28.1987,28.};
               dcenter = {3.22493,3.05678,
3.03523};
               mat = {
                  0.0967104,-0.00794298,0.0212789,0.,0.022713,0.0340542,-0.090517,
0.,-5.6998e-05,0.0929845,0.0349681,0.,0.,0.,0.,1.
               };
               xlate = {-3.75481,-3.38461,
-3.03523};
               center = {29.75,28.1987,28.};
            };
         };
         Lights {
            Lights = {
               {
                  type="BiDirectional"
               },,,};
         };
         Camera {
            Camera {
               auto_norm = "None";
            };
         };
         View {
            View {
               renderer => "Software";
               trigger = 1;
            };
            ViewUI {
               ViewPanel {
                  UI {
                     panel {
                        defaultX = 560;
                        defaultY = 461;
                     };
                  };
               };
            };
         };
      };
   };
   MODS.Read_Field Read_Field<NEx=341.,NEy=22.> {
      read_field_ui {
         panel {
            option {
               set = 1;
            };
         };
         file_browser {
            x = 501;
            y = 300;
            width = 286;
            height = 368;
            cancel = 1;
         };
         portable = 0;
         filename = "./test/data.fld";
      };
      DVread_field {
         Mesh_Unif+Node_Data Output_Field;
      };
      Read_Field_Param {
         one_time = 0;
      };
      loop {
         done = 1;
      };
      do_loop {
         set_run {
            output = 0;
         };
      };
   };
   MODS.bounds bounds<NEx=22.,NEy=33.> {
      in_field => <-.Read_Field.field;
      BoundsUI {
         DVnode_data_labels {
            labels[];
         };
         UIradioBoxLabel {
            label_cmd {
               cmd[];
            };
         };
      };
   };
   HLM.Basic_Axis3D Basic_Axis3D<NEx=33.,NEy=352.> {
      axis {
         xform {
            xlate = {-7.,-7.,-7.};
         };
      };
      panel {
         option {
            set = 0;
         };
      };
      Axis_UI<instanced=0> {
         probe_edit {
            GDxform_editor {
               x_trans = -7.;
               y_trans = -7.;
               z_trans = -7.;
               abs_x_trans = -7.;
               abs_y_trans = -7.;
               abs_z_trans = -7.;
            };
            XformEditorUI {
               trans_shell {
                  x = 407;
                  y = 254;
                  ok = 1;
               };
            };
         };
      };
      Axis_scale_X = 10.;
      Axis_scale_Y = 10.;
      Axis_scale_Z = 10.;
   };
   MODS.orthoslice orthoslice<NEx=286.,NEy=99.> {
      OrthoSliceParam {
         axis = 2;
         plane => 32;
      };
      in_field => <-.Read_Field.field;
      DVorthoslice {
         DVorthoslice_unif {
            out {
               nnode_data = 7;
            };
         };
      };
   };
   MODS.set_minmax set_minmax<NEx=286.,NEy=176.> {
      in_field => <-.orthoslice.out_fld;
      SetMinmaxParam {
         comp = 5;
         min => -1.;
         max => 1.;
      };
      SetMinMaxUI {
         min_typein {
            x = 0;
         };
         max_typein {
            x = 0;
         };
      };
   };
   MODS.isoline isoline<NEx=286.,NEy=264.> {
      in_field => <-.set_minmax.out_fld;
      IsoParam {
         contour_comp = 5;
         ncontours = 18;
         level_min => -1.;
         level_max => 1.;
      };
   };
   MODS.orthoslice orthoslice#1<NEx=649.,NEy=99.> {
      in_field => <-.Read_Field.field;
      OrthoSliceParam {
         axis = 2;
         plane => 32;
      };
   };
   MODS.downsize downsize<NEx=649.,NEy=165.> {
      in_field => <-.orthoslice#1.out_fld;
      DownsizeParam {
         factor0 = 3.;
         factor1 = 3.;
      };
   };
   MODS.combine_vect combine_vect<NEx=649.,NEy=220.> {
      in_field => <-.downsize.out_fld;
      CombVectUI {
         DVnode_data_labels {
            labels[];
         };
         UIoptionBoxLabel {
            label_cmd {
               cmd[];
            };
         };
      };
   };
   GEOMS.Arrow1 Arrow1<NEx=693.,NEy=275.>;
   MODS.glyph glyph<NEx=649.,NEy=330.> {
      in_field => <-.combine_vect.out_fld;
      in_glyph => <-.Arrow1.out_fld;
      GlyphParam {
         scale = 100.;
      };
      GlyphUI {
         scale_slider {
            min = -100.;
            max = 100.;
         };
      };
   };
   MODS.orthoslice orthoslice#2<NEx=462.,NEy=99.> {
      in_field => <-.Read_Field.field;
      OrthoSliceParam {
         axis = 2;
         plane => 32;
      };
   };
   MODS.set_minmax set_minmax#1<NEx=462.,NEy=176.> {
      in_field => <-.orthoslice#2.out_fld;
      SetMinmaxParam {
         comp = 5;
         min => -0.01;
         max => 0.01;
      };
      SetMinMaxUI {
         min_typein {
            x = 0;
         };
         max_typein {
            x = 0;
         };
         DVnode_data_labels {
            labels[];
         };
         UIradioBoxLabel {
            label_cmd {
               cmd[];
            };
         };
      };
   };
   MODS.isovolume isovolume<NEx=462.,NEy=264.> {
      in_field => <-.set_minmax#1.out_fld;
      IsoVolParam {
         iso_component = 5;
         iso_level => -1.;
         map_component = {5};
      };
      DVcell_data_labels {
         labels[];
      };
      DVnode_data_labels {
         labels[];
      };
      IsoVolUI {
         cut_level {
            min => -1.;
            max => 1.;
         };
         UIoptionBoxLabel {
            label_cmd {
               cmd[7] = {
                  ,,,,,
                  {
                     set=1,,,
                  },
               };
            };
         };
         UIradioBoxLabel {
            label_cmd {
               cmd[];
            };
         };
         UIoptionBoxLabel_cell {
            label_cmd {
               cmd[];
               outItem = ;
            };
         };
      };
   };
   GEOMS.LegendVert LegendVert<NEx=462.,NEy=330.> {
      obj_in => <-.isovolume.out_obj;
      font_text {
         text = "-adobe-helvetica-medium-r-*-*-20-*-*-*-*-*-*-*";
      };
      LabelsMacro {
         labelsOffset = 0.23;
      };
      x_min = 0.75;
      x_max = 0.8;
   };
   MODS.set_minmax set_minmax#2<NEx=88.,NEy=99.> {
      in_field => <-.Read_Field.field;
      SetMinmaxParam {
         comp = 4;
         min => -0.1;
         max => 0.1;
      };
      SetMinMaxUI {
         min_typein {
            x = 0;
         };
         max_typein {
            x = 0;
         };
         DVnode_data_labels {
            labels[];
         };
         UIradioBoxLabel {
            label_cmd {
               cmd[];
            };
         };
      };
   };
   MODS.isosurface isosurface<NEx=88.,NEy=176.> {
      in_field => <-.set_minmax#2.out_fld;
      IsoParam {
         iso_component = 3;
         map_component = {4};
      };
      DVcell_data_labels {
         labels[];
      };
      DVnode_data_labels {
         labels[];
      };
      IsoUI {
         UIoptionBoxLabel {
            label_cmd {
               cmd[7] = {
                  ,,,,
                  {
                     set=1,,,
                  },,
               };
            };
         };
         UIradioBoxLabel {
            label_cmd {
               cmd[];
            };
         };
         UIoptionBoxLabel_cell {
            label_cmd {
               cmd[];
               outItem = ;
            };
         };
      };
   };
   GEOMS.LegendHoriz LegendHoriz<NEx=88.,NEy=264.> {
      obj_in => <-.isosurface.out_obj;
   };
   HLM.OutputImage OutputImage<NEx=341.,NEy=484.> {
      view_in => <-.Uviewer3D.Scene_Selector.curr_view;
      output_field {
         write_image_ui {
            panel {
               option {
                  set = 0;
               };
            };
         };
      };
   };
   GDM.OutputVPS OutputVPS<NEx=506.,NEy=484.> {
      view_in => <-.Uviewer3D.Scene_Selector.curr_view;
      vpsOutput {
         backMode = "Black";
      };
      vpsUI {
         vpsColor {
            OPcmdList = {
               {
                  set=1,,,,,,,,,,,,,,,,,,,,
               },};
         };
         vpsBgmode {
            OPcmdList = {,
               {
                  set=1,,,,,,,,,,,,,,,,,,,,
               },};
         };
         vpsOrient {
            OPcmdList = {
               {
                  set=1,,,,,,,,,,,,,,,,,,,,
               },};
         };
         vpsSize {
            OPcmdList = {
               
               {
                  set=1,,,,,,,,,,,,,,,,,,,,
               },,,,,
               
            };
         };
         vpsCSub {
            OPcmdList = {
               {
                  set=1,,,,,,,,,,,,,,,,,,,,
               },,};
         };
         vpsDepthSort {
            OPcmdList = {
               {
                  set=1,,,,,,,,,,,,,,,,,,,,
               },};
         };
         vpsGMode {
            OPcmdList = {,
               {
                  set=1,,,,,,,,,,,,,,,,,,,,
               },};
         };
      };
   };
   ANIM_MODS.image_capture image_capture<NEx=176.,NEy=484.> {
      imcapCompute {
         ImageCap {
            LGDView => <-.<-.<-.Uviewer3D.Scene_Selector.curr_view;
         };
      };
   };
};
