import { memo, SVGProps } from 'react';

const Ellipse17Icon = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 600 600' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path
      d='M600 300C600 465.685 465.685 600 300 600C134.315 600 0 465.685 0 300C0 134.315 134.315 0 300 0C465.685 0 600 134.315 600 300ZM65.9874 300C65.9874 429.242 170.758 534.013 300 534.013C429.242 534.013 534.013 429.242 534.013 300C534.013 170.758 429.242 65.9874 300 65.9874C170.758 65.9874 65.9874 170.758 65.9874 300Z'
      fill='#D9D9D9'
    />
  </svg>
);

const Memo = memo(Ellipse17Icon);
export { Memo as Ellipse17Icon };
