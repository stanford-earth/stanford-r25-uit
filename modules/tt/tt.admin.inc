<?php

function _tt_get_stored_pta_number($index) {
  $container = variable_get('stanford_r25_room_pta_numbers',
    array(1 => array('stanford_r25_room_pta_numbers' => '1095428-220-AABPK')));
  if ($index > count($container)) {
    $index = 1;
  }
  return $container[$index]['stanford_r25_room_pta_numbers'];
}

// enter pta number form in admin.inc
function _tt_pta($form, &$form_state)
{

// start with an instructive string
  $markup_str = 'Shirley, please add instructions here ';

  $form['description'] = array(
    '#markup' => t($markup_str),
  );

  $form['#tree'] = true;
  if (empty($form_state['number_ptas'])) {
    $container = variable_get('stanford_r25_room_pta_numbers',
      array(1 => array()));
    $form_state['number_ptas'] = count($container);
  }

  for ($i = 1; $i <= $form_state['number_ptas']; $i++) {
    $form['pta_numbers'][$i] = array(
      '#type' => 'container',
    );
    $form['pta_numbers'][$i]['stanford_r25_room_pta_numbers'] = array(
      '#type' => 'stanford_pta',
      '#title' => t('PTA number '),
      '#description' => t('Enter provostial PTA number(s) that require approval for reservation'),
      '#default_value' => _tt_get_stored_pta_number($i),
      //'#default_value' => variable_get('stanford_r25_room_pta_numbers', '1095428-220-AABPK'),
      '#required' => FALSE,
    );
  }

  $form['add_item'] = array(
    '#type'  => 'submit',
    '#value' => t('Add another PTA'),
    '#submit' => array('_tt_pta_add_pta'),
  );

  /*
  // PTA numbers that needs approval
  $form['stanford_r25_room_pta_numbers'] = array(
    '#type' => 'textfield',
    '#title' => t('PTA numbers '),
    '#description' => t('Enter provostial PTA number(s) that require approval for reservation'),
    '#default_value' => variable_get('stanford_r25_room_pta_numbers', '1095428-220-AABPK'),
    '#required' => FALSE,
  );
  */

  // email message to requestor if they are using Provostial PTA to be added by Tech Training
  $default_body = "Your core rental request has been received. " .
    "Please send us a brief description of how the event meets the required " .
    "eligibility criteria for a core rental. Tech Training will promptly " .
    "review your request's eligibility and notify you with a decision. " .
    "If the event does not qualify as a core funded rental, " .
    "you may use your departmental PTA for the reservation.";
  $default_provost_pta = variable_get('stanford_r25_provostial_pta',
    array('value' => $default_body, 'format' => filter_default_format()));
  $form['stanford_r25_provostial_pta'] = array(
    '#type' => 'text_format',
    '#title' => t('Message when using Provostial PTA'),
    '#description' => t('Add text .'),
    '#default_value' => $default_provost_pta['value'],
    '#format' => $default_provost_pta['format'],
  );

  // submit button
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

function _tt_pta_add_pta($form, &$form_state) {
  $form_state['number_ptas']++;
  $form_state['rebuild'] = true;
}

function _tt_pta_submit($form, &$form_state) {
  variable_set('stanford_r25_room_pta_numbers',
    $form_state['values']['pta_numbers']);
  variable_set('stanford_r25_provostial_pta',
    $form_state['values']['stanford_r25_provostial_pta']);
}