<?php
// $Id$

/**
 * Filter by order state.
 */
class commerce_order_handler_filter_order_state extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('State');

      foreach (commerce_order_states() as $name => $state) {
        $options[$name] = $state->title;
      }
      $this->value_options = $options;
    }
  }

  function op_simple() {
    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();
    $status = commerce_order_status_load(reset($this->value));
    $states = commerce_order_statuses(array('state' => $status->state));

    $this->query->add_where($this->options['group'], "$this->table_alias.status", array_keys($states), $this->operator);
  }

  function op_empty() {
    $this->ensure_my_table();
    if ($this->operator == 'empty') {
      $operator = "IS NULL";
    }
    else {
      $operator = "IS NOT NULL";
    }

    $this->query->add_where($this->options['group'], "$this->table_alias.status", NULL, $operator);
  }
}
