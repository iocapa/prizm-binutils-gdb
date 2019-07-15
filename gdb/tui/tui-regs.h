/* TUI display registers in window.

   Copyright (C) 1998-2019 Free Software Foundation, Inc.

   Contributed by Hewlett-Packard Company.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef TUI_TUI_REGS_H
#define TUI_TUI_REGS_H

#include "tui/tui-data.h"

/* A data item window.  */

struct tui_data_item_window : public tui_gen_win_info
{
  tui_data_item_window ()
    : tui_gen_win_info (DATA_ITEM_WIN)
  {
  }

  DISABLE_COPY_AND_ASSIGN (tui_data_item_window);

  tui_data_item_window (tui_data_item_window &&) = default;

  void rerender () override;

  void refresh_window () override;

  const char *name = nullptr;
  /* The register number, or data display number.  */
  int item_no = -1;
  bool highlight = false;
  gdb::unique_xmalloc_ptr<char> content;
};

/* The TUI registers window.  */
struct tui_data_window : public tui_win_info
{
  tui_data_window ()
    : tui_win_info (DATA_WIN)
  {
  }

  DISABLE_COPY_AND_ASSIGN (tui_data_window);

  void refresh_all () override;

  void refresh_window () override;

  const char *name () const override
  {
    return DATA_NAME;
  }

  void check_register_values (struct frame_info *frame);

  void show_registers (struct reggroup *group);

  struct reggroup *get_current_group () const
  {
    return current_group;
  }

protected:

  void do_scroll_vertical (int num_to_scroll) override;
  void do_scroll_horizontal (int num_to_scroll) override
  {
  }

  void rerender () override;

private:

  /* Display the registers in the content from 'start_element_no'
     until the end of the register content or the end of the display
     height.  No checking for displaying past the end of the registers
     is done here.  */
  void display_registers_from (int start_element_no);

  /* Display the registers starting at line line_no in the data
     window.  Answers the line number that the display actually
     started from.  If nothing is displayed (-1) is returned.  */
  int display_registers_from_line (int line_no);

  /* Return the index of the first element displayed.  If none are
     displayed, then return -1.  */
  int first_data_item_displayed ();

  /* Display the registers in the content from 'start_element_no' on
     'start_line_no' until the end of the register content or the end
     of the display height.  This function checks that we won't
     display off the end of the register display.  */
  void display_reg_element_at_line (int start_element_no, int start_line_no);

  void show_register_group (struct reggroup *group,
			    struct frame_info *frame,
			    int refresh_values_only);

  /* Answer the number of the last line in the regs display.  If there
     are no registers (-1) is returned.  */
  int last_regs_line_no () const;

  /* Answer the line number that the register element at element_no is
     on.  If element_no is greater than the number of register
     elements there are, -1 is returned.  */
  int line_from_reg_element_no (int element_no) const;

  /* Answer the index of the first element in line_no.  If line_no is
     past the register area (-1) is returned.  */
  int first_reg_element_no_inline (int line_no) const;

  /* Delete all the item windows in the data window.  This is usually
     done when the data window is scrolled.  */
  void delete_data_content_windows ();

  void erase_data_content (const char *prompt);

  /* Windows that are used to display registers.  */
  std::vector<tui_data_item_window> regs_content;
  int regs_column_count = 0;
  struct reggroup *current_group = nullptr;
};

#endif /* TUI_TUI_REGS_H */
