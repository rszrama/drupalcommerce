<?php
// $Id$

/**
 * Defines a handler area that provides payment totals, the order balance, and
 *   a form to add new payments.
 */
class commerce_payment_handler_area_totals extends views_handler_area {
  function init(&$view, $options) {
    parent::init($view, $options);

    $this->additional_fields['amount'] = 'amount';
    $this->additional_fields['currency_code'] = 'currency_code';
    $this->additional_fields['status'] = 'status';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['add_payment_form'] = array('default' => TRUE);

    return $options;
  }

  /**
   * Provide the checkbox for enabling the Add payment form..
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['add_payment_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display an add payment form in the totals area when using a single order argument.'),
      '#description' => t('The argument should be setup using a Views relationship on the transaction Order ID.'),
      '#default_value' => $this->options['add_payment_form'],
    );
  }

  function render($empty = FALSE) {
    if (!$empty) {
      // Calculate a total of successful payments for each currency.
      $totals = array();

      foreach ($this->view->result as $result) {
        if ($result->commerce_payment_transaction_status == 'success') {
          if (isset($totals[$result->commerce_payment_transaction_currency_code])) {
            $totals[$result->commerce_payment_transaction_currency_code] += $result->commerce_payment_transaction_amount;
          }
          else {
            $totals[$result->commerce_payment_transaction_currency_code] = $result->commerce_payment_transaction_amount;
          }
        }
      }

      $rows = array();

      // If there's only a single currency total...
      if (count($totals) == 1) {
        $currency_code = array_shift(array_keys($totals));

        // Add a row for the total amount paid.
        $rows[] = array(
          'data' => array(
            array('data' => t('Total paid'), 'class' => array('label')),
            array('data' => commerce_currency_format($totals[$currency_code], $currency_code), 'class' => array('total')),
          ),
          'class' => array('total-paid'),
        );

        // Add a row for the remaining balance on the order.
        $rows[] = array(
          'data' => array(
            array('data' => t('Order balance'), 'class' => array('label')),
            // TODO: Provide an API function to caluclate an order's remaining balance.
            array('data' => '-', 'class' => array('balance')),
          ),
          'class' => array('order-balance'),
        );
      }
      else {
        foreach ($totals as $currency_code => $amount) {
          $rows[] = array(
            'data' => array(
              array('data' => t('Total paid (@currency_code)', array('@currency_code' => $currency_code)), 'class' => array('label')),
              array('data' => commerce_currency_format($amount, $currency_code), 'class' => array('total')),
            ),
            'class' => array('total-paid', 'total-' . $currency_code),
          );
        }
      }

      // Build and render the form to add a payment if the View contains a
      // single order argument.
      if ($this->options['add_payment_form'] &&
        in_array('order_id', array_keys($this->view->argument)) &&
        !in_array('order_id_1', array_keys($this->view->argument)) &&
        !empty($this->view->args[$this->view->argument['order_id']->position])) {

        // Load the specified order and add the form if the order exists.
        if ($order = commerce_order_load($this->view->args[$this->view->argument['order_id']->position])) {
          module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.forms');

          $form = drupal_render(drupal_get_form('commerce_payment_order_transaction_add_form', $order));
        }
      }
      else {
        $form = NULL;
      }

      // Prepare variables for use in the theme function.
      $variables = array(
        'rows' => $rows,
        'form' => $form,
        'view' => $this->view,
        'totals' => $totals,
      );

      return theme('commerce_payment_totals', $variables);
    }
  }
}