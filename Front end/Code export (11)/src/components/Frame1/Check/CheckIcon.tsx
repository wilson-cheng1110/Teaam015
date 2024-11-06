import { memo, SVGProps } from 'react';

const CheckIcon = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 18 13' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path d='M6.55 13L0.85 7.3L2.275 5.875L6.55 10.15L15.725 0.975L17.15 2.4L6.55 13Z' fill='#1D1B20' />
  </svg>
);

const Memo = memo(CheckIcon);
export { Memo as CheckIcon };
