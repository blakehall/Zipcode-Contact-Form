<?php

/**
 * Page callback for admin/settings/zcf
 */
function zcf_admin_listing() {
  // Print a table listing of contact records.
  // Query zcf_info table print id, name, email, phone.
  $zcf = db_select('zcf_info', 'z')
    ->fields('z')
    ->execute();

  while ($result = $zcf->fetchAssoc()) {
    $view_link = l('View', 'admin/settings/zcf/' .$result['zcf_info_id']);
    $edit_link = l('Edit', 'admin/settings/zcf/' .$result['zcf_info_id'] .'/edit');
    $row[] = array($result['zcf_info_id'], $result['name'], $result['email'], $result['phone'], $view_link, $edit_link);
  }

  // Add a link to create a new contact record.
  $output = l('Create new contact', 'admin/settings/zcf/new') .'<br />';
  // Setup the output for the listing table.
  $header = array('id', 'name', 'email', 'phone', '', '');
  $output .= theme('table', array('header' => $header, 'rows' => $row));
  return $output;
}

/**
 * Page callback for admin/settings/zcf/new
 */
function zcf_admin_create($form, &$form_state, $id = NULL) {

  $form['zcf_info_id'] = array(
    '#type' => 'hidden',
    '#value' => $id['zcf_info_id'],
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#size' => 20,
    '#default_value' => $id['name'],
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#size' => 40,
    '#default_value' => $id['email'],
  );
  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#size' => 20,
    '#default_value' => $id['phone'],
  );
  $form['zipcodes'] = array(
    '#type' => 'textarea',
    '#title' => t('Zipcodes'),
    '#description' => t('Enter a comma seperated list of zipcodes'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function zcf_admin_create_validate($form, &$form_state) {
  // TODO we may want to check validity of zipcodes here.
//  $form_state['values']['zipcodes'];
}

function zcf_admin_create_submit($form, &$form_state) {
  // Save our record.
  $values = $form_state['values'];

  $zcf_record = db_merge('zcf_info')
    ->key(array('zcf_info_id' => $values['zcf_info_id']))
    ->fields(array(
      'name' => $values['name'],
      'email' => $values['email'],
      'phone' => $values['phone'],
    ))
    ->execute();

  // If $zcf_record == 2 an update has been performed, but if it returns 1 it's an insert.
  if ($zcf_record == 1) {
    $record = db_select('zcf_info', 'z')
      ->fields('z')
      ->condition('name', $values['name'], '=')
      ->condition('email', $values['email'], '=')
      ->condition('phone', $values['phone'], '=')
      ->execute()
      ->fetchAssoc();
  
    $record_id = $record['zcf_info_id'];
  }
  else {
    $record_id = $values['zcf_info_id'];
  }

  drupal_set_message('Contact record Saved');

  // We also need to properly process and save our zipcode data in the zcf table.
  $zipcodes = explode(',', $values['zipcodes']);

  foreach ($zipcodes as $i => $zipcode) {
    db_merge('zcf')
      ->key(array('zipcode' => trim($zipcode)))
      ->fields(array(
        'zcf_info_id' => $record_id,
      ))
      ->execute();
      drupal_set_message('Saved info for zip: ' .$zipcode);
  }

  // Redirect back to the listing page.
  drupal_goto('admin/settings/zcf');
}

/**
 * Page callback to show one particular contact record.
 *
 * Ideally this would be returned through a theme function,
 * that is registered with hook_theme().
 */
function zcf_record_show($id) {
  $output = $id['name'] .'<br />';
  $output .= $id['email'] .'<br />';
  $output .= $id['phone'] .'<br />';
  return $output;
}
