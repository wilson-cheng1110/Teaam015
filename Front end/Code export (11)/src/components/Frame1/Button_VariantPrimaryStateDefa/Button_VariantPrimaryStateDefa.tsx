import { memo } from 'react';
import type { FC, ReactNode } from 'react';

import resets from '../../_resets.module.css';
import { Star_Size16 } from '../Star_Size16/Star_Size16.js';
import { X_Size16 } from '../X_Size16/X_Size16.js';
import classes from './Button_VariantPrimaryStateDefa.module.css';

interface Props {
  className?: string;
  classes?: {
    root?: string;
  };
  text?: {
    button?: ReactNode;
  };
}
/* @figmaId 163:547 */
export const Button_VariantPrimaryStateDefa: FC<Props> = memo(function Button_VariantPrimaryStateDefa(props = {}) {
  return (
    <button className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      {props.text?.button != null ? props.text?.button : <div className={classes.button}>Button</div>}
    </button>
  );
});
